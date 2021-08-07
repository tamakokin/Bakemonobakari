// ばけものチーム
// ステージ3のカメラの移動処理を管理する
// 更新履歴
// 2021/08/06 更新者：陳　作成

#include "CameraControl_Stage3.h"
#include "CameraSpline.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

#define ASPECT  1.77777f

// Sets default values
ACameraControl_Stage3::ACameraControl_Stage3():
	m_NowDistance(600.0f),
	m_NowSpeed(1.0f),
	m_Player(true),
	m_pPlayerActor(NULL),
	m_AdjustmentPos(FVector(0.0f, 70.0f, 0.0f)),
	m_TargetPos(FVector::ZeroVector),
	m_SpeedHight(5.0f),
	m_SpeedWidth(3.0f),
	m_Distance(0.0f),
	m_MaxSpeed(14.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraControl_Stage3::BeginPlay()
{
	Super::BeginPlay();
	
	// 速度と距離の初期化
	m_NowSpeed = m_SpeedWidth;

	// プレイヤーアクターを検索する
	SearchPlayer();

	// プレイヤーを注目アクターにする
	SetTargetPlayerActor();
}

// Called every frame
void ACameraControl_Stage3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Player)
	{
		MovePlayerCamera();
	}
	else
	{
		MoveCamera();
	}

}

// プレイヤーアクターを検索する-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl_Stage3::SearchPlayer()
{
	// プレイヤーアクターを探索する
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	// プレイヤーを取得する
	for (int i = 0; i < actors.Num(); i++)
	{
		//if (actors[i]->ActorHasTag("Spline"))
		//{
			//m_pSpline.Add(Cast<ACameraSpline>(actors[i]));
		//}

		if (actors[i]->ActorHasTag("Player"))
		{
			m_pPlayerActor = actors[i];
		}
	}
}

// カメラのプレイヤー追従の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl_Stage3::MovePlayerCamera()
{
	//if (m_pSpline.Num() == 0)return;

	//SearchSpline();

	// カメラとプレイヤーの相対距離
	float relativeDistance = m_pPlayerActor->GetActorLocation().X  - GetActorLocation().X;
	if (GEngine)
	{
		FString TheFloatStr = FString::SanitizeFloat(relativeDistance);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *TheFloatStr);
	}
	
	// カメラの移動を行うか判定する
	if (FMath::Abs(relativeDistance) > 300)
	{
		//if ((relativeDistance > 0) && (m_pPlayerActor->GetActorRotation().Yaw >= 0))
		//{
			m_FrontPos = FVector(70.0f, 0.0f, 0.0f);
		//}
		//else if ((relativeDistance < 0) && (m_pPlayerActor->GetActorRotation().Yaw <= -80.0f))
		//{
			//m_FrontPos = FVector(0.0f, -m_AdjustmentPos.Y, m_AdjustmentPos.Z);
		//}
	}
	else
	{
		m_FrontPos = FVector(0.0f, 0.0f, 0.0f);
	}

	// 移動後の目標座標を設定
	//FVector targetPos = FVector(0.0f, 0.0f, 0.0f);//need fixed
	//FVector move = FVector((m_pPlayerActor->GetActorLocation().X - 300.0), GetActorLocation().Y, GetActorLocation().Z);
	FVector move = FVector((m_pPlayerActor->GetActorLocation().X - 300.0), m_pPlayerActor->GetActorLocation().Y * 0.9, GetActorLocation().Z);

	// 横移動分を加算
	//targetPos += m_FrontPos;
	//float speed = (targetPos.X - GetActorLocation().X) / m_NowSpeed;

	//if ((speed > 0.0f) && (speed > m_MaxSpeed))
	//{
		//speed = m_MaxSpeed;
	//}
	//else if ((speed < 0.0f) && (speed < -m_MaxSpeed))
	//{
		//speed = -m_MaxSpeed;
	//}

	//move += FVector(speed, 0.0f, 0.0f);

	// 水平移動分を加算
	//move += FVector(0.0f, m_pPlayerActor->GetActorLocation().Y * 0.8, 0.0f);
	//move += FVector(0.0f, 0.0f, (targetPos.Z - GetActorLocation().Z) / m_SpeedHight);

	// 移動
	SetActorLocation(move);
}

// カメラのプレイヤー追従以外の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl_Stage3::MoveCamera()
{
	//if (!m_pActor)return;
	// 移動後の目標座標を設定
	//FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_Distance, m_pActor->GetActorLocation().Y, m_pActor->GetActorLocation().Z);

	//FVector move = (targetPos - GetActorLocation()) / m_NowSpeed;

	//SetActorLocation(GetActorLocation() + move);
}

// 引数で渡された座標がカメラに映るかどうかを調べる-----------------------------------------------------------------------------------------------------------------------------------------------
bool ACameraControl_Stage3::CheckInCamera(FVector _pos, FVector _size)
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