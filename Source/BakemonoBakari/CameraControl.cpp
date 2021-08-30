// 2021 05/01 大金巧侑 カメラの移動処理を管理する
// 2021 05/10 大金巧侑 カメラ内に入っているかを調べる
// 2021 05/10 大金巧侑 初期値を設定
// 2021 05/14 大金巧侑 カメラ内に入っているかを調べる更新
// 2021/08/25 松中海斗 静止時にカメラを引く、移動時にカメラを近づけるように更新
// 2021/08/27 松中海斗 スプラインがない場合プレイヤーをターゲットにするように更新

#include "CameraControl.h"
#include "CameraSpline.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

#define ASPECT  1.77777f

ACameraControl::ACameraControl() :
	m_pActor(NULL),
	m_NowDistance(600.0f),
	m_NowSpeed(8.0f),
	m_Player(true),
	m_CountTime(0.0f),
	m_bCount(false),
	m_pPlayerActor(NULL),
	m_TargetPos(FVector::ZeroVector),
	m_SpeedHight(5.0f),
	m_SpeedWidth(3.0f),
	m_ScaleUpTime(3.0f),
	m_ScaleDownTime(2.0f),
	m_Distance(600.0f),
	m_Distance_ScaleUpMagnification(0.5f),
	m_LenghWidth(220.0f),
	m_LenghHight(100.0f),
	m_MaxSpeed(14.0f),
	m_Move(false),
	m_PrevMove(false),
	m_Right(true),
	m_PrevChangeCameraPos(FVector::ZeroVector),
	m_PrevCameraPos(FVector::ZeroVector),
	m_PrevIsJump(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// 初期化処理//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::BeginPlay()
{
	Super::BeginPlay();

	// 速度と距離の初期化
	m_NowSpeed = m_SpeedWidth;

	// アクターを検索する
	Search();

	// プレイヤーを注目アクターにする
	SetTargetPlayerActor();

	for (int n = 0; n < m_pSpline.Num(); ++n)
	{
		m_pSpline[n]->Start();
	}

	SearchSpline();


	// カメラの初期位置を初期化
	SetActorLocation(FVector(m_pPlayerActor->GetActorLocation().X + m_Distance, m_pPlayerActor->GetActorLocation().Y, m_pPlayerActor->GetActorLocation().Z));

	m_PrevCameraPos, m_PrevChangeCameraPos = (GetActorLocation());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// アクターを検索する-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::Search()
{
	// アクターを探索する
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	for (int i = 0; i < actors.Num(); i++)
	{
		// スプラインを探す
		if (actors[i]->ActorHasTag("Spline"))
		{
			m_pSpline.Add(StaticCast<ACameraSpline*>(actors[i]));
		}

		// プレイヤーを取得する
		else if (actors[i]->ActorHasTag("PlayerCharacter"))
		{
			m_pPlayerActor = StaticCast<ABakemonoBakariCharacter*>(actors[i]);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// スプラインを検索する-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::SearchSpline()
{
	if (m_pSpline.Num() > 0)
	{
		// マップ内のスプラインを取得
		TArray<FVector> tempPoses;
		TArray<float> tempLengthes;

		FVector playerPos = m_pPlayerActor->GetActorLocation();

		for (int n = 0; n < m_pSpline.Num(); ++n)
		{
			FVector tempPos = m_pSpline[n]->GetTargetPos();
			float y = playerPos.Y - tempPos.Y;
			float z = playerPos.Z - tempPos.Z;

			float length = FMath::Sqrt(y * y + z * z);

			tempPoses.Add(tempPos);
			tempLengthes.Add(length);
		}

		// バブルソートで一番近いスプラインの座標を取得
		for (int n = 0; n < tempPoses.Num(); ++n)
		{
			for (int i = tempPoses.Num() - 1; i > n; --i)
			{
				if (tempLengthes[i] < tempLengthes[i - 1])
				{
					float temp = tempLengthes[i];
					FVector tempPos = tempPoses[i];
					ACameraSpline* tempSpline = m_pSpline[i];

					tempLengthes[i] = tempLengthes[i - 1];
					tempPoses[i] = tempPoses[i - 1];
					m_pSpline[i] = m_pSpline[i - 1];

					tempLengthes[i - 1] = temp;
					tempPoses[i - 1] = tempPos;
					m_pSpline[i - 1] = tempSpline;
				}
			}
		}
		//m_TargetPos = tempPoses[0];
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Player)
	{
		MovePlayerCamera(DeltaTime);
	}
	else
	{
		MoveCamera();
	}

	// カメラが地面に当たった時の揺れ
	if ((m_SwayingNow > m_SwayingMax) && (m_IsSway))
	{
		m_SwayingNow = 0.0f;
		m_IsSway = false;
	}
	else if (m_IsSway)
	{
		m_SwayingNow += m_SwayingAdd;
	}

	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, m_SwayingNow));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// プレイヤーの着地時にカメラを揺らす-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::Swaying()
{
	UE_LOG(LogTemp, Warning, TEXT("AAAAAA"));

	// 地面に当たった時カメラを揺らす
	m_SwayingNow = -m_SwayingMax;
	m_IsSway = true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// カメラのプレイヤー追従の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera(float _deltaTime)
{
	/*	if (m_pSpline[0])
		{
			m_TargetPos = m_pSpline[0]->GetTargetPos();
		}*/
	//	// カメラとプレイヤーの相対距離
	//	float relativeDistance = m_TargetPos.Y - GetActorLocation().Y;
	//
	//
	//	// 移動後の目標座標を設定（松中・変更）
	//	FVector targetPos = m_TargetPos;
	//	targetPos.X = (m_TargetPos.X + m_Distance);
	//
	//	FVector move = FVector().ZeroVector;
	//
	//	// 横移動を行う場合
	//	if (m_Move)
	//	{	// 横移動分を加算
	//		targetPos += m_FrontPos;
	//
	//		float speed = (targetPos.Y - GetActorLocation().Y) / m_NowSpeed;
	//
	//		if ((speed > 0.0f) && (speed > m_MaxSpeed))
	//		{
	//			speed = m_MaxSpeed;
	//		}
	//		else if ((speed < 0.0f) && (speed < -m_MaxSpeed))
	//		{
	//			speed = -m_MaxSpeed;
	//		}
	//
	//		move.Y = speed;
	//	}
	//	else
	//	{
	//		// カメラの移動を行うか判定する
	//		if (FMath::Abs(relativeDistance) > m_LenghWidth)
	//		{
	//			if ((relativeDistance > 0) && (m_pPlayerActor->GetActorRotation().Yaw >= 0))
	//			{
	//				m_FrontPos = FVector(0.0f, m_AdjustmentPos.Y, m_AdjustmentPos.Z);
	//
	//				m_Right = false;
	//
	//				m_Move = true;
	//
	//			}
	//			else if ((relativeDistance < 0) && (m_pPlayerActor->GetActorRotation().Yaw <= -80.0f))
	//			{
	//				m_FrontPos = FVector(0.0f, -m_AdjustmentPos.Y, m_AdjustmentPos.Z);
	//
	//				m_Right = true;
	//
	//				m_Move = true;
	//
	//			}
	//		}
	//	}
	//
	//	// 縦移動分を加算
	//	move.Z = (targetPos.Z - GetActorLocation().Z) / m_SpeedHight;
	//
	//	// コントローラーからの入力によってカメラを移動
	//	move += m_MoveCamera;
	//
	//	// カメラをもとの座標に戻す
	//	m_MoveCamera -= FVector(0.0f, m_MoveCamera.Y / m_CameraMoveReturn.Y, m_MoveCamera.Z / m_CameraMoveReturn.Z);
	//
	//	// 移動
	//	SetActorLocation(GetActorLocation() + move);
	//
	//	// 時間をカウント（松中）
	//	m_CountTime += _deltaTime;

	//SearchSpline();

	if (m_pSpline[0])
	{
		m_TargetPos = m_pSpline[0]->GetTargetPos();
	}
	// カメラとプレイヤーの相対距離
	float relativeDistance = m_TargetPos.Y - GetActorLocation().Y;

	// モードが切り替わったら初期化（松中）
	if (m_PrevMove != m_Move || m_PrevIsJump != m_pPlayerActor->GetIsJump())
	{
		m_PrevMove = m_Move;
		m_PrevIsJump = m_pPlayerActor->GetIsJump();
		m_CountTime = 0.0f;
		m_PrevChangeCameraPos = GetActorLocation();
	}

	FVector move = FVector().ZeroVector;

	// 移動を行う場合
	if (m_Move)
	{
		// 移動後の目標座標を設定（松中・変更）
		FVector targetPos = m_TargetPos;
		targetPos.X = (m_TargetPos.X + m_Distance) - (m_TargetPos.Z + m_Distance) * (m_Distance_ScaleUpMagnification - 1.0f);
		targetPos.Z -= m_TargetPos.Z * (m_Distance_ScaleUpMagnification - 1.0f);

		// 奥行きの設定
		if (m_pPlayerActor->GetIsJump())
		{
			FVector locationTmp = GetActorLocation();
			float alpha = FMath::Clamp((m_CountTime / m_ScaleDownTime), 0.0f, 1.0f);
			locationTmp.X = FMath::InterpSinInOut(m_PrevChangeCameraPos.X, (m_TargetPos.X + m_Distance), alpha);
			locationTmp.Z = FMath::InterpSinInOut(m_PrevChangeCameraPos.Z, m_TargetPos.Z, alpha);
			SetActorLocation(locationTmp);
		}
		else
		{
			FVector locationTmp = GetActorLocation();
			locationTmp.X = FMath::InterpSinInOut(m_PrevChangeCameraPos.X, targetPos.X, FMath::Clamp((m_CountTime / m_ScaleUpTime), 0.0f, 1.0f));
			SetActorLocation(locationTmp);
		}

		float speed = (targetPos.Y - GetActorLocation().Y) / m_NowSpeed;

		if ((speed > 0.0f) && (speed > m_MaxSpeed))
		{
			speed = m_MaxSpeed;
		}
		else if ((speed < 0.0f) && (speed < -m_MaxSpeed))
		{
			speed = -m_MaxSpeed;
		}

		move.Y = speed;

		// 縦移動分を加算
		move.Z = (targetPos.Z - GetActorLocation().Z) / m_SpeedHight;

		// 移動
		SetActorLocation(GetActorLocation() + move);

		// カメラの移動が止まったら移動フラグをオフ（松中）
		if (m_PrevCameraPos == GetActorLocation()) m_Move = false;

		m_PrevCameraPos = GetActorLocation();
		m_PrevIsJump = m_pPlayerActor->GetIsJump();
	}
	else
	{
		// 奥行きの設定（松中）
		FVector locationTmp = GetActorLocation();
		float alpha = FMath::Clamp((m_CountTime / m_ScaleDownTime), 0.0f, 1.0f);
		locationTmp.X = FMath::InterpSinInOut(m_PrevChangeCameraPos.X, (m_TargetPos.X + m_Distance), alpha);
		locationTmp.Z = FMath::InterpSinInOut(m_PrevChangeCameraPos.Z, m_TargetPos.Z, alpha);
		SetActorLocation(locationTmp);

		// カメラの移動を行うか判定する
		if (FMath::Abs(relativeDistance) > m_LenghWidth)
		{
			if ((relativeDistance > 0) && (m_pPlayerActor->GetActorRotation().Yaw >= 0))
			{
				m_FrontPos = FVector(0.0f, m_AdjustmentPos.Y, m_AdjustmentPos.Z);
				m_Right = false;
				m_Move = true;
			}
			else if ((relativeDistance < 0) && (m_pPlayerActor->GetActorRotation().Yaw <= -80.0f))
			{
				m_FrontPos = FVector(0.0f, -m_AdjustmentPos.Y, m_AdjustmentPos.Z);
				m_Right = true;
				m_Move = true;
			}
		}
	}
	
	// 時間をカウント（松中）
	m_CountTime += _deltaTime;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// カメラのプレイヤー追従以外の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MoveCamera()
{
	if (!m_pActor)return;
	// 移動後の目標座標を設定
	FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_Distance, m_pActor->GetActorLocation().Y, m_pActor->GetActorLocation().Z);

	FVector move = (targetPos - GetActorLocation()) / m_NowSpeed;

	SetActorLocation(GetActorLocation() + move);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// 点Aと点Bの距離を計る-----------------------------------------------------------------------------------------------------------------------------------------------
float ACameraControl::Measurement(FVector _a, FVector _b)
{
	float y = FMath::Pow(_a.Y - _b.Y, 2);
	float z = FMath::Pow(_a.Z - _b.Z, 2);

	float lengh = FMath::Sqrt(y + z);
	return lengh;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// 引数で渡された座標がカメラに映るかどうかを調べる-----------------------------------------------------------------------------------------------------------------------------------------------
bool ACameraControl::CheckInCamera(FVector _pos, FVector _size)
{
	// カメラとの距離
	float distance = _pos.X - GetActorLocation().X;

	float width = distance;
	float hight = distance / ASPECT;

	// アクターのサイズに併せる
	width -= _size.X;
	hight -= _size.Y;

	FVector2D pos = FVector2D(GetActorLocation().Y - _pos.Y, GetActorLocation().Z - _pos.Z);

	// 2021 05 07 条件が間違っていたため修正
	if ((FMath::Abs(pos.X) > FMath::Abs(width)) || (FMath::Abs(pos.Y) > FMath::Abs(hight)))
	{
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
