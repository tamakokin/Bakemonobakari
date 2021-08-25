// Shirikodama.cpp
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
//			：大金巧侑
// 作成日	：2021/07/09
// 更新履歴	：

#include "Shirikodama.h"

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

	Move();
}

void AShirikodama::MoveStart(float _speed, float _risingPower)
{
	m_MoveSpeed = _speed;
	m_RisingPower = _risingPower;
	SetActorHiddenInGame(false);
	m_pMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// 移動処理------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::Move()
{
	if (!bHidden)
	{
		m_Count++;

		// 移動量
		FVector move = FVector::ZeroVector;

		// 横移動
		move.Y += m_MoveSpeed;

		if (!m_IsPlayerAttack)
		{
			// 縦移動
			m_Gravity = 0.0098f * m_Count * m_Count;
			m_RisingPowerNow = m_RisingPower - m_Gravity;
			move.Z += m_RisingPowerNow;
		}
		SetActorLocation(GetActorLocation() + move);

		if (m_IsSquashed)
		{
			m_Count = 0.0f;

			m_IsSquashed = false;
		}

		// 一定の座標を超えると消える
		if (GetActorLocation().Y > m_LimitPosY)
		{
			// 削除処理
			Des();
		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
// 削除処理------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::Des()
{
	m_pMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	m_Count = 0;
	m_IsSquashed = false;
}
//------------------------------------------------------------------------------------------------------------------------------------------

// オーバーラップ関数------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// アクターのポインターがぬるでなければ
	if (OtherActor != NULL)
	{
		// 地面に当たった時バウンドする
		if (OtherActor->ActorHasTag("Ground"))
		{
			if (m_RisingPowerNow < 0.0f)
			{
				m_IsSquashed = true;
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------