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
	m_pFiring = Cast<UENEMY_TYPE_FIRING_Component>(GetOwner());
	m_pJump   = Cast<UENEMY_TYPE_JUMP_Component>(GetOwner());

	if (m_pEnemy->m_pCapsuleComp)
	{
		m_pEnemy->m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &UENEMY_JUMP_SHOT_Component::BeginHit);
	}

	// 現在の座標の高さを取得
	m_OldHeight = m_pEnemy->GetActorLocation().Z;
}


// Called every frame
void UENEMY_JUMP_SHOT_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_Air)return;

	if (m_pEnemy->GetActorLocation().Z >= m_OldHeight + m_ShotHeight)
	{
		m_pFiring->EnemyAttack();
		m_Air = true;
	}
}

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_JUMP_SHOT_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")))
	{
		m_Air = false;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

