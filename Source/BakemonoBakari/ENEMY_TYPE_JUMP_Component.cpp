// ばけものチーム
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_JUMP_Component::UENEMY_TYPE_JUMP_Component() :
	m_JumpWait(0.0f),
	m_IsGravity(false),
	m_IsJump(false),
	m_InitVelocityZ(12.0f),
	m_JumpGravity(20.0f),
	m_pEnemy(NULL),
	m_EnemyGravityDeltaTime(0.0f),
	m_StraightVector(false),
	m_SwitchVector(false),
	m_IsAir(false),
	m_IsHit(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UENEMY_TYPE_JUMP_Component::BeginPlay()
{
	Super::BeginPlay();
	m_pEnemy = Cast<AEnemyActor>(GetOwner());
}

// Called every frame
void UENEMY_TYPE_JUMP_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return;
	if (m_pEnemy->m_pCapsuleComp->GetCollisionEnabled() == AACF_NONE)return;

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	if (m_pEnemy->m_pEnemyMesh)
	{
		m_pEnemy->m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::BeginHit);
		m_pEnemy->m_pEnemyMesh->OnComponentEndOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::EndHit);
	}

	// 重力の移動のみを行う
	if (m_IsGravity) 
	{
		FVector tempPos = FVector::ZeroVector;

		tempPos += Gravity(DeltaTime);

		//// エネミーの座標を変える
		m_pEnemy->SetActorLocation(tempPos + m_pEnemy->GetActorLocation(), true);
	}

	//EnemyMove(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UENEMY_TYPE_JUMP_Component::EnemyMove(float _deltaTime)
{
	FVector tempPos = FVector::ZeroVector;

	// 待機時間を超えるとジャンプ開始
	tempPos += Jump(_deltaTime);
	tempPos += Gravity(_deltaTime);

	//// エネミーの座標を変える
	m_pEnemy->SetActorLocation(tempPos + m_pEnemy->GetActorLocation(), true);
}

FVector UENEMY_TYPE_JUMP_Component::Jump(float _deltaTime)
{
	// 一時的に保持する座標
	FVector tempEnemyPosition = FVector::ZeroVector;
	tempEnemyPosition.Z += m_InitVelocityZ;

	// エネミーの状態を変える
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);

	return tempEnemyPosition;
}

FVector UENEMY_TYPE_JUMP_Component::Gravity(float _deltaTime)
{
	// 一時的に保持する座標
	FVector tempEnemyPosition = FVector::ZeroVector;

	// 鉛直投げ上げ
	float tempDeltaZ = 0.0f;

	// 時間をカウント
	m_EnemyGravityDeltaTime += _deltaTime;

	// 重力移動
	tempDeltaZ = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));
	tempEnemyPosition.Z += tempDeltaZ;

	// 値の反映
	return tempEnemyPosition;
}

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")))
	{
		float gravity = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));

		if (FMath::Abs(gravity) > m_InitVelocityZ)
		{
			m_EnemyGravityDeltaTime = 0.0f;
			m_IsAir = false;
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 地面から離れたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
