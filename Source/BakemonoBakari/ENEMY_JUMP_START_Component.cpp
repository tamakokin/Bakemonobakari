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
	m_pJumpComponent = Cast<UENEMY_TYPE_JUMP_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_JUMP_Component::StaticClass()));
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
	m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
}

// Called every frame
void UENEMY_JUMP_START_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((!m_pJumpComponent) || (!m_pStraightComponent) || (!m_pEnemyRote)||(!m_pEnemy)) 
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		m_pJumpComponent = Cast<UENEMY_TYPE_JUMP_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_JUMP_Component::StaticClass()));
		m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
		m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
		return;
	}

	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;


	// 時間が来たらジャンプさせる
	if (m_JumpCount == m_JumpStartTiming) 
	{
		// エネミーの状態を変える
		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);

		if (m_pEnemyRote) 
		{
			m_pEnemyRote->EnemyRote();
			m_pJumpComponent->m_IsAir = true;
			m_pJumpComponent->m_IsJump = true;
			m_JumpCount = 0;
		}
	}

	// 移動処理
	if (m_pEnemy->GetEnmeyState() == AEnemyActor::ENEMY_STATE_MOVE)
	{
		m_pJumpComponent->EnemyMove(DeltaTime);

		// 空中での処理
		if (m_pJumpComponent->GetIsAir())
		{
			m_pStraightComponent->EnemyMove();
		}
	}

	// 地面出の処理
	if (!m_pJumpComponent->GetIsAir())
	{
		// 地面にいる間にカウントする
		m_JumpCount++;
	}
}

