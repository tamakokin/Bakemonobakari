// ばけものチーム
#include "ENEMY_HITODAMA_Component.h"
#include "EnemyActor.h"


// Sets default values for this component's properties
UENEMY_HITODAMA_Component::UENEMY_HITODAMA_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UENEMY_HITODAMA_Component::BeginPlay()
{
	Super::BeginPlay();

	m_MoveCount = FMath::RandRange(0, 360);
	m_Re = false;

	// コンポーネントの取得
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	if (m_pEnemy)
	{
		m_StartPos = m_pEnemy->GetActorLocation();
	}
}


// Called every frame
void UENEMY_HITODAMA_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	m_MoveCount += m_Speed;

	FVector movePos = FVector::ZeroVector;

	if (m_pEnemy)
	{
		if (m_Re)
		{
			movePos = FVector(0.0f, FMath::Cos(FMath::DegreesToRadians(m_MoveCount)) * m_Distance - m_Distance, FMath::Sin(FMath::DegreesToRadians(m_MoveCount)) * m_Distance);
		}
		else
		{
			movePos = FVector(0.0f, -FMath::Cos(FMath::DegreesToRadians(m_MoveCount)) * m_Distance + m_Distance, FMath::Sin(FMath::DegreesToRadians(m_MoveCount)) * m_Distance);
		}

		m_pEnemy->SetActorLocation(m_StartPos + movePos);

		if (m_MoveCount >= 360)
		{
			m_MoveCount = 0;
			m_Re = !m_Re;
		}
	}
}

