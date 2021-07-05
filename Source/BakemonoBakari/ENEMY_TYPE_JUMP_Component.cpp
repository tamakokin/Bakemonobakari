// ばけものチーム
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_JUMP_Component::UENEMY_TYPE_JUMP_Component() :
	m_IsAir(true)
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

	EnemyMove(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UENEMY_TYPE_JUMP_Component::EnemyMove(float _deltaTime)
{
	// 待機時間を超えるとジャンプ開始
	if (m_IsJump)
	{
		Jump(_deltaTime);
	}
	if (m_IsGravity)
	{
		Gravity(_deltaTime);
	}
}

void UENEMY_TYPE_JUMP_Component::Jump(float _deltaTime)
{
	// 一時的に保持する座標
	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
	tempEnemyPosition.Z += m_InitVelocityZ;

	// 値の反映
	m_pEnemy->SetActorLocation(tempEnemyPosition, true);

	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
}

void UENEMY_TYPE_JUMP_Component::Gravity(float _deltaTime)
{
	if (!m_IsAir)return;

	// 一時的に保持する座標
	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();

	// 鉛直投げ上げ
	float tempDeltaZ = 0.0f;

	// 時間をカウント
	m_EnemyGravityDeltaTime += _deltaTime;

	// 重力移動
	tempDeltaZ = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));
	tempEnemyPosition.Z += tempDeltaZ;

	// 値の反映
	m_pEnemy->SetActorLocation(tempEnemyPosition, true);
}

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")))
	{
		m_EnemyGravityDeltaTime = 0.0f;
		m_IsAir = false;
		m_IsJump = false;
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
