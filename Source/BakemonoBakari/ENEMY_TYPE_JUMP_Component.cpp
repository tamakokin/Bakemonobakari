// ばけものチーム
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_JUMP_Component::UENEMY_TYPE_JUMP_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UENEMY_TYPE_JUMP_Component::BeginPlay()
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
void UENEMY_TYPE_JUMP_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}

	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY) 
	{
		m_EnemyJumpDeltaTime = 0.0f;
		return;
	}

	EnemyMove(DeltaTime);
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UENEMY_TYPE_JUMP_Component::EnemyMove(float _deltaTime)
{
	// 時間をカウント
	m_EnemyJumpDeltaTime += _deltaTime;

	// 待機時間を超えるまで動かない
	if (m_EnemyJumpDeltaTime < m_JumpWait)	return;

	// 一時的に保持する座標
	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();

	// 一時的に保持する回転
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	// 方向転換
	if (m_SwitchVector)
	{
		if (!m_StraightVector)
		{
			// 前向き
			tempEnemyRotation.Yaw = 90.f;
		}
		else
		{
			// 後ろ向き
			tempEnemyRotation.Yaw = 270.f;
		}

		m_SwitchVector = false;
		m_pEnemy->SetActorRotation(tempEnemyRotation);
	}

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

	// 可動範囲を超えたら
	if ((tempEnemyPosition.Y > m_MaxMoveY) || (tempEnemyPosition.Y < m_MinMoveY))
	{
		m_SwitchVector = true;
		m_StraightVector = !m_StraightVector;
	}

	// 鉛直投げ上げ
	float tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
	tempEnemyPosition.Z += tempDeltaZ;

	// 初期位置より下に行った場合補正してリターン
	if (tempEnemyPosition.Z < m_pEnemy->GetStartPos().Z)
	{
		// 初期位置の戻ったら時間の初期化
		m_pEnemy->SetActorLocation(tempEnemyPosition);
		m_EnemyJumpDeltaTime = 0.f;
		return;
	}

	// 値の反映
	m_pEnemy->SetActorLocation(tempEnemyPosition);

	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
}