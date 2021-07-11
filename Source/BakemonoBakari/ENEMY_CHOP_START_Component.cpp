// ばけものチーム
// かまいたちの行動処理

#include "ENEMY_TYPE_JUMP_Component.h"
#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "Enemy_Rote_Component.h"
#include "LINETRACE_Component.h"
#include "ENEMY_CHOP_START_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_CHOP_START_Component::UENEMY_CHOP_START_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UENEMY_CHOP_START_Component::BeginPlay()
{
	Super::BeginPlay();
	m_pEnemy = Cast<AEnemyActor>(GetOwner());
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
	m_pJumpComponent = Cast<UENEMY_TYPE_JUMP_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_JUMP_Component::StaticClass()));
	m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
}
// Called every frame
void UENEMY_CHOP_START_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if ((!m_pJumpComponent) || (!m_pStraightComponent) || (!m_pEnemy))return;

	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return;
	if (m_pEnemy->m_pCapsuleComp->GetCollisionEnabled() == AACF_NONE)return;

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	EnemyMove(DeltaTime);
}

// プレイヤーに近づいて斬りつける-----------------------------------------------------------------------------------------------------------------------------------
void UENEMY_CHOP_START_Component::EnemyMove(float _deltaTime)
{
	++m_AttackCount;

	if (m_AttackCount == m_AttackStartTiming)
	{
		m_pEnemyRote->EnemyRote();
		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
	}

	if (m_AttackCount >= m_AttackStartTiming)
	{
		if (!m_IsAttack)
		{
			m_pStraightComponent->EnemyMove(_deltaTime);
			if (m_HitGround)
			{
				m_pEnemyRote->TurnRote();
			}

			// 移動中にプレイヤーがあったら攻撃
			if (m_HitPlayer)
			{
				m_IsAttack = true;
				m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
			}
		}
	}

	if (!m_IsAttack)
	{
		if (m_AttackCount >= m_AttackEndTiming)
		{
			m_HitPlayer = false;
			m_HitGround = false;

			m_AttackCount = 0;

			m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);

		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------------

// 攻撃終了--------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_CHOP_START_Component::FinishAttack()
{
	m_IsAttack = false;
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);
	UE_LOG(LogTemp,Warning,TEXT("ABC"));
}
// --------------------------------------------------------------------------------------------------------------------------------------------------


