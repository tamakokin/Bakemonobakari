// ばけものチーム
// 2021/07/05 大金巧侑
// 一定時間ごとに横方向にジャンプする敵のコントロール

#include "ENEMY_JUMP_START_Component.h"
#include "ENEMY_TYPE_JUMP_Component.h"
#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "Enemy_Rote_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_JUMP_START_Component::UENEMY_JUMP_START_Component():
	m_JumpStartTiming(200)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UENEMY_JUMP_START_Component::BeginPlay()
{
	Super::BeginPlay();

	m_pEnemy = Cast<AEnemyActor>(GetOwner());
}

// Called every frame
void UENEMY_JUMP_START_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((!m_pJumpComponent) || (!m_pStraightComponent))return;

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	// 死亡時に処理差させない
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return;
	if (!m_pEnemy->m_pEnemyMesh->GetVisibleFlag())return;

	// 時間が来たらジャンプさせる
	if (m_JumpCount == m_JumpStartTiming) 
	{
		if (m_pEnemyRote) 
		{
			m_pEnemyRote->EnemyRote();
			m_pJumpComponent->m_IsAir = true;
			m_JumpCount++;
		}
	}

	// 空白から地面についた際の処理
	if (m_JumpCount >= m_JumpStartTiming)
	{
		m_pJumpComponent->EnemyMove(DeltaTime);
		m_pStraightComponent->EnemyMove(DeltaTime);
	}

	if (m_JumpCount > m_JumpStartTiming)
	{
		if (!m_pJumpComponent->GetIsAir())
		{
			m_JumpCount = 0.0f;
		}
	}

	// 地面に付いている間カウントする
	if (!m_pJumpComponent->GetIsAir())
	{
		m_JumpCount++;
	}
}

