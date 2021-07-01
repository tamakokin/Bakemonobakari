// ばけものチーム
// 真っ直ぐ突っ込んで来る敵の行動処理

#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_STRAIGHT_Component::UENEMY_TYPE_STRAIGHT_Component() :
	m_prevEnemyPosition(FVector::ZeroVector)
	, m_StraightVector(false)
	, m_SwitchVector(false)
	, m_EnemyMovingDistance(0.0f)
	, m_pEnemy(NULL)
	, m_ChangeVectorTolerance(0.0f)
	, m_moveSpeedY(0.0f)
	, m_MaxMoveY(0.0f)
	, m_MinMoveY(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UENEMY_TYPE_STRAIGHT_Component::BeginPlay()
{
	Super::BeginPlay();

	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	// 移動できる範囲の座標を決定
	if (m_pEnemy)
	{
		m_MaxMoveY += m_pEnemy->GetActorLocation().Y;
		m_MinMoveY += m_pEnemy->GetActorLocation().Y;
	}
}

// Called every frame
void UENEMY_TYPE_STRAIGHT_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return;

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	EnemyMove(DeltaTime);

	if (m_pEnemy->GetOverlappedActor())
	{
		if (m_pEnemy->GetOverlappedActor()->ActorHasTag("PlayerCharacter"))
		{
			// 動きを止める
			EnemyStop();
			m_pEnemy->SetOverlappedActor(NULL);
		}
	}
}

void UENEMY_TYPE_STRAIGHT_Component::EnemyMove(float _deltaTime)
{
	if (!m_pEnemy)return;

	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	// 方向転換
	if (m_SwitchVector)
	{
		if (!m_StraightVector)
		{
			// 前向き
			tempEnemyRotation.Yaw = 0.f;
		}
		else
		{
			// 後ろ向き
			tempEnemyRotation.Yaw = 180.f;
		}

		m_SwitchVector = false;
		m_pEnemy->SetActorRotation(tempEnemyRotation);
	}

	m_prevEnemyPosition = tempEnemyPosition;

	// 進む方向に応じて
	if (!m_StraightVector)
	{
		// 前進
		tempEnemyPosition.Y += m_moveSpeedY;
	}
	else
	{
		// 後退
		tempEnemyPosition.Y -= m_moveSpeedY;
	}

	m_EnemyMovingDistance += m_moveSpeedY;

	// 修正後のアクター位置を設定
	m_pEnemy->SetActorLocation(tempEnemyPosition,true);

	// 可動範囲を超えたら
	if ((tempEnemyPosition.Y > m_MaxMoveY) || (tempEnemyPosition.Y < m_MinMoveY))
	{
		m_SwitchVector = true;
		m_StraightVector = !m_StraightVector;
	}

	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
}


// プレイヤーに当たった時の処理
void UENEMY_TYPE_STRAIGHT_Component::EnemyStop()
{
	// 方向転換
	if (m_EnemyMovingDistance > m_ChangeVectorTolerance)
	{
		//m_SwitchVector = true;
		m_StraightVector = !m_StraightVector;
	}
}
