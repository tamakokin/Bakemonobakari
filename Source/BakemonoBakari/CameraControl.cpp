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
					m_pSpline[i-1] = tempSpline;
				}
			}
		}


		//// 誤差許容範囲
		//float errorValue = 10.0f;

		//// 二つのスプラインの間にプレイヤーがいるかを調べる
		//if ((playerPos.Y >= tempPoses[0].Y - errorValue) && (playerPos.Y <= tempPoses[0].Y + errorValue))
		//{
		//	if ((playerPos.Y >= tempPoses[1].Y - errorValue) && (playerPos.Y <= tempPoses[1].Y + errorValue))
		//	{
		//		if (((tempPoses[0].Z < playerPos.Z) && (tempPoses[1].Z > playerPos.Z)) || ((tempPoses[0].Z > playerPos.Z) && (tempPoses[1].Z < playerPos.Z)))
		//		{
		//			m_TargetPos.Z = playerPos.Z;
		//		}
		//	}
		//}
		//if ((playerPos.Y < tempPoses[0].Y - errorValue) || (playerPos.Y > tempPoses[0].Y + errorValue))
		//{
		//	m_TargetPos.Y = playerPos.Y;
		//}
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
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// カメラのプレイヤー追従の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera(float _deltaTime)
{
	//SearchSpline();
	if (m_pSpline[0])
	{
		UE_LOG(LogTemp, Warning, TEXT("AAAAAA"));

		m_TargetPos = m_pSpline[0]->GetTargetPos();
	}
	// カメラとプレイヤーの相対距離
	float relativeDistance = m_TargetPos.Y - GetActorLocation().Y;


	// 移動後の目標座標を設定（松中・変更）
	FVector targetPos = m_TargetPos;
	targetPos.X = (m_TargetPos.X + m_Distance);

	FVector move = FVector().ZeroVector;

	// 横移動を行う場合
	if (m_Move)
	{	// 横移動分を加算
		targetPos += m_FrontPos;

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
	}
	else
	{
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

	// 縦移動分を加算
	move.Z = (targetPos.Z - GetActorLocation().Z) / m_SpeedHight;

	// コントローラーからの入力を加算
	move += m_MoveCamera;

	// 移動
	SetActorLocation(GetActorLocation() + move);

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
