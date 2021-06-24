// ばけものチーム
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_JUMP_Component::UENEMY_TYPE_JUMP_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UENEMY_TYPE_JUMP_Component::BeginPlay()
{
	Super::BeginPlay();

	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	// 移動できる範囲の座標を決定
	if (m_pEnemy)
	{
		m_MaxMoveY += m_pEnemy->GetActorLocation().Y;
		m_MinMoveY += m_pEnemy->GetActorLocation().Y;
	}

	if (m_pEnemy->m_pCapsuleComp)
	{
		m_pEnemy->m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::BeginHit);
		m_pEnemy->m_pCapsuleComp->OnComponentEndOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::EndHit);
	}
}

// Called every frame
void UENEMY_TYPE_JUMP_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY) 
	{
		m_EnemyJumpDeltaTime = 0.0f;
		return;
	}
	if (m_pEnemy->m_pCapsuleComp->GetCollisionEnabled() == AACF_NONE)return;

	EnemyMove(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UENEMY_TYPE_JUMP_Component::EnemyMove(float _deltaTime)
{
	// 一時的に保持する座標
	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();

	// 一時的に保持する回転
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	// 時間をカウント
	m_EnemyJumpDeltaTime += _deltaTime;

	// 方向転換
	if (m_SwitchVector)
	{
		if (!m_StraightVector)
		{
			// 前向き
			tempEnemyRotation.Roll = 0.f;
		}
		else
		{
			// 後ろ向き
			tempEnemyRotation.Roll = 180.f;
		}

		m_SwitchVector = false;
		m_pEnemy->SetActorRotation(tempEnemyRotation);
	}

	// 進む方向に応じて
	if (!m_StraightVector)
	{
		// 前進
		tempEnemyPosition.Y += m_moveSpeedY;
	}
	else
	{
		// 後退
		tempEnemyPosition.Y -= m_moveSpeedY;
	}

	// 可動範囲を超えたら
	if ((tempEnemyPosition.Y > m_MaxMoveY) || (tempEnemyPosition.Y < m_MinMoveY))
	{
		m_SwitchVector = true;
		m_StraightVector = !m_StraightVector;
	}

	// 鉛直投げ上げ
	float tempDeltaZ = 0.0f;
	// 待機時間を超えるとジャンプ開始
	if (m_EnemyJumpDeltaTime >= m_JumpWait)
	{
		m_EnemyJumpDeltaTime = 0.0f;
		tempEnemyPosition.Z += m_InitVelocityZ;
	}

	if (m_IsAir)
	{
		// 時間をカウント
		m_EnemyGravityDeltaTime += _deltaTime;

		// 重力移動
		tempDeltaZ = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));
		tempEnemyPosition.Z += tempDeltaZ;
	}

	// 値の反映
	m_pEnemy->SetActorLocation(tempEnemyPosition, true);

	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
}

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground"))||(OverlappedComponent->ComponentHasTag("Block")))
	{
		m_EnemyJumpDeltaTime = 0.0f;
		m_EnemyGravityDeltaTime = 0.0f;
		m_IsAir = false;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::EndHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")))
	{
		m_IsAir = true;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
