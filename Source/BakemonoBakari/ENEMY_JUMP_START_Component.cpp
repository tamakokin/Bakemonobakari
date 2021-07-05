// ばけものチーム
// 2021/07/05 大金巧侑
// 一定時間ごとに横方向にジャンプする敵のコントロール

#include "ENEMY_JUMP_START_Component.h"
#include "ENEMY_TYPE_JUMP_Component.h"
#include "ENEMY_TYPE_STRAIGHT_Component.h"
// Sets default values for this component's properties
UENEMY_JUMP_START_Component::UENEMY_JUMP_START_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_pJumpComponent     = CreateDefaultSubobject<UENEMY_TYPE_JUMP_Component>(TEXT("Jump"));

	m_pStraightComponent = CreateDefaultSubobject<UENEMY_TYPE_STRAIGHT_Component>(TEXT("Straight"));
}


// Called when the game starts
void UENEMY_JUMP_START_Component::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UENEMY_JUMP_START_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((!m_pJumpComponent) || (m_pStraightComponent))return;
	m_JumpCount++;

	if (m_JumpCount == m_JumpStartTiming) 
	{
		m_pJumpComponent->SetJump(true);
		m_pStraightComponent->SetIsMove(true);
	}
	if (m_JumpCount > m_JumpStartTiming)
	{
		if (!m_pJumpComponent->GetIsAir())
		{
			m_pStraightComponent->SetIsMove(false);
			m_JumpCount = 0;
		}
	}
}

