// ばけものチーム


#include "ENEMY_TYPE_DASH_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_DASH_Component::UENEMY_TYPE_DASH_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UENEMY_TYPE_DASH_Component::BeginPlay()
{
	Super::BeginPlay();

	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	m_UpSpeed = 0.3f;
}

// Called every frame
void UENEMY_TYPE_DASH_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_pEnemy->GetIsAction())
	{
		// 移動速度を加速
		m_Speed += m_UpSpeed;
		FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
		tempEnemyPosition.Y += m_Speed;

		// 移動
		m_pEnemy->SetActorLocation(tempEnemyPosition, false);

		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
	}
	else
	{
		m_Speed = 0.0f;
	}
}

