// ばけものチーム
// ジャンプして、一旦空中で停止し球を撃つエネミー

#include "ENEMY_JUMP_SHOT_Component.h"
#include "ENEMY_TYPE_FIRING_Component.h"
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_JUMP_SHOT_Component::UENEMY_JUMP_SHOT_Component():
	m_ShotHeight(100.0f),
	m_Air(false),
	m_OldHeight(0.0f),
	m_pEnemy(NULL),
	m_pJump(NULL),
	m_pFiring(NULL)
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UENEMY_JUMP_SHOT_Component::BeginPlay()
{
	Super::BeginPlay();
	// ...
	m_pEnemy  = Cast<AEnemyActor>(GetOwner());

	m_pFiring = Cast<UENEMY_TYPE_FIRING_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_FIRING_Component::StaticClass()));
	m_pJump   = Cast<UENEMY_TYPE_JUMP_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_JUMP_Component::StaticClass()));
	// 現在の座標の高さを取得
	m_OldHeight = m_pEnemy->GetActorLocation().Z;
}

// Called every frame
void UENEMY_JUMP_SHOT_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_pEnemy->m_pEnemyMesh)
	{
		m_pEnemy->m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &UENEMY_JUMP_SHOT_Component::BeginHit);
	}

	if (m_Air)return;
	if ((!m_pEnemy)||(!m_pFiring))return;

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	if (m_pEnemy->GetActorLocation().Z >= m_OldHeight + m_ShotHeight)
	{
		m_pFiring->EnemyAttack();

		m_pJump->m_IsJump = false;
		m_pJump->m_IsGravity = true;

		m_Air = true;
	}
}

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_JUMP_SHOT_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;
	if (OtherActor->ActorHasTag("Enemy"))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")))
	{
		m_pJump->m_IsJump = true;
		m_pJump->m_IsGravity = false;

		m_Air = false;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

