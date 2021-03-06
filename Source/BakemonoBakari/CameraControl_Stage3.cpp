// ばけものチーム
// ステージ3のカメラの移動処理を管理する
// 更新履歴
// 2021/08/06 更新者：陳　作成

#include "CameraControl_Stage3.h"
#include "CameraSpline.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

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
	m_MaxSpeed(14.0f),
	m_rotate(20.0f)
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
}

// プレイヤーアクターを検索する
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
		if (actors[i]->ActorHasTag("Player"))
		{
			m_pPlayerActor = actors[i];
		}
	}
}

// カメラのプレイヤー追従の移動を行う
void ACameraControl_Stage3::MovePlayerCamera()
{

	// カメラとプレイヤーの相対距離
	float relativeDistance = m_pPlayerActor->GetActorLocation().Y  - GetActorLocation().Y;
	if (GEngine)
	{
		FString TheFloatStr = FString::SanitizeFloat(relativeDistance);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *TheFloatStr);
	}
	
	// カメラの移動を行うか判定する
	if (FMath::Abs(relativeDistance) > 300)
	{
		m_FrontPos = FVector(70.0f, 0.0f, 0.0f);
	}
	else
	{
		m_FrontPos = FVector(0.0f, 0.0f, 0.0f);
	}

	float move_X = m_pPlayerActor->GetActorLocation().X - 300.0f;
	float move_Y = m_pPlayerActor->GetActorLocation().Y * 0.9f;
	float move_Z = GetActorLocation().Z;

	float rotate_Pitch;

	if (FMath::Abs(m_pPlayerActor->GetActorLocation().Y - GetActorLocation().Y) >= 200.0f)
	{
		if (m_pPlayerActor->GetActorLocation().Y > 0.0f)
		{
			//move_Y = m_pPlayerActor->GetActorLocation().Y - 200.0f;
		}
		else
		{
			//move_Y = m_pPlayerActor->GetActorLocation().Y + 200.0f;
		}
	}
	else
	{
		//move_Y = m_pPlayerActor->GetActorLocation().Y * 0.8;
	}


	if (m_pPlayerActor->GetActorLocation().Z < GetActorLocation().Z)
	{
		rotate_Pitch = 10.0 * (m_rotate - FMath::Abs(m_pPlayerActor->GetActorLocation().Z)) / m_rotate;
		//rotate_Pitch = 0.0f;
	}
	else
	{
		rotate_Pitch = 10.0f;
	}

	

	// 移動後の目標座標を設定
	FVector move = FVector(move_X, move_Y, move_Z);
	// 回転後の目標座標を設定
	FRotator rotate = FRotator(-10.0f + rotate_Pitch, 0.0f, 0.0f);

	// 移動
	SetActorLocation(move);
	//回転
	//SetActorRotation(rotate);
	
}