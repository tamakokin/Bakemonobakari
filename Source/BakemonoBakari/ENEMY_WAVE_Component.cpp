// ばけものチーム
// ウェーブさせる敵

#include "ENEMY_WAVE_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_WAVE_Component::UENEMY_WAVE_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UENEMY_WAVE_Component::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントの取得
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	if (m_pEnemy)
	{
		m_StartPos = m_pEnemy->GetActorLocation();
	}
}


// Called every frame
void UENEMY_WAVE_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_pEnemy)
	{
		// 座標を保持
		FVector movePos = m_pEnemy->GetActorLocation();

		// 現在の横の座標
		float y = FMath::Abs(movePos.Y);

		// sinを使って上下移動させるための角度
		float rad = FMath::DegreesToRadians((int)y % 360);

		m_pEnemy->SetActorLocation(FVector(0.0f, movePos.Y, m_StartPos.Z + FMath::Sin(rad) * m_Distance));
	}
}

