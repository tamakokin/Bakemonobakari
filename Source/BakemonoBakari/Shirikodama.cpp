// Shirikodama.cpp
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
//			：大金巧侑
// 作成日	：2021/07/09
// 更新履歴	：

#include "Shirikodama.h"
#include "CheckInScreen.h"

AShirikodama::AShirikodama() :
	m_MoveSpeed(10.0f),
	m_RisingPower(12.0f),
	m_RisingPowerNow(0.0f),
	m_Gravity(0.0f),
	m_Count(0),
	m_pMesh(NULL)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShirikodama::BeginPlay()
{
	Super::BeginPlay();

	m_pCheckInScreen = Cast<UCheckInScreen>(GetComponentByClass(UCheckInScreen::StaticClass()));

	// メッシュを探す
	m_pMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (m_pMesh)
	{
		m_pMesh->OnComponentBeginOverlap.AddDynamic(this, &AShirikodama::BeginHit);
	}
}

void AShirikodama::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_pCheckInScreen)return;

	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		Destroy();
	}

	m_Count += DeltaTime;

	m_SquashedCount++;

	//if (m_IsSquashed) 
	//{
	//	if (m_SquashedCount < 10)
	//	{
	//		SetActorScale3D(GetActorScale3D() - FVector(0.0f, -0.05f, 0.05f));
	//	}
	//	else
	//	{
	//		SetActorScale3D(GetActorScale3D() + FVector(0.0f, -0.05f, 0.05f));
	//	}

	//	if (m_SquashedCount == 20)
	//	{
	//		SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));

	//		m_IsSquashed = false;
	//		m_SquashedCount = 0;
	//	}
	//}

	// 移動量
	FVector move = FVector::ZeroVector;

	// 横移動
	move.Y += m_MoveSpeed;

	// 縦移動
	m_Gravity = 9.8f * 0.5f * (m_Count * m_Count);
	m_RisingPowerNow = m_RisingPower - m_Gravity;
	move.Z += m_RisingPowerNow;

	SetActorLocation(GetActorLocation() + move);
}

void AShirikodama::MoveStart(float _speed, float _risingPower)
{
	m_MoveSpeed = _speed;
	m_RisingPower = _risingPower;
}

// オーバーラップ関数
void AShirikodama::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// アクターのポインターがぬるでなければ
	if (OtherActor != NULL)
	{
		if (OtherActor->ActorHasTag("Ground")) 
		{
			if (m_RisingPowerNow < 0.0f)
			{
				m_Count = 0.0f;

				m_IsSquashed = true;
			}
		}
	}
}