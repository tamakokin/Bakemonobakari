// ばけものチーム
// 真っ直ぐ突っ込んで来る敵の行動処理

#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "EnemyActor.h"
#include "Enemy_Rote_Component.h"

// Sets default values for this component's properties
UENEMY_TYPE_STRAIGHT_Component::UENEMY_TYPE_STRAIGHT_Component() :
	 m_IsTurn(true)
	,m_TurnTiming(50)
	,m_MoveSpeed(2.0f)
	,m_MaxMoveY(400.0f)
	,m_MinMoveY(-400.0f)
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
	m_pRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));

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
	if ((!m_pEnemy) || (!m_pRote))
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		m_pRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
		return;
	}

	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;

	// 直接移動関数を呼ぶ
	if (m_IsMove)
	{
		EnemyMove();
	}
}

void UENEMY_TYPE_STRAIGHT_Component::EnemyMove()
{
	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;

	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();

	// 回転を行っている時
	if (m_pRote->GetRoteCount() > 0)
	{
		m_IsTurn = false;
	}
	// 回転をしていない時
	else 
	{
		// 可動範囲を超えたら
		if ((tempEnemyPosition.Y > m_MaxMoveY) || (tempEnemyPosition.Y < m_MinMoveY))
		{
			// 回転を許可されている場合
			if (m_IsTurn)
			{
				m_pRote->TurnRote();
			}
		}

		// 回転できるようになるまでをカウント
		m_TurnCount++;

		// 回転をできるようにする
		if (m_TurnCount > m_TurnTiming)
		{
			m_TurnCount = 0;
			m_IsTurn = true;
		}

		// 進む方向に応じて
		if (m_pEnemy->GetActorRotation().Yaw >= 170.0f)
		{
			// 前進
			tempEnemyPosition.X -= m_MoveSpeed;
		}
		else if (m_pEnemy->GetActorRotation().Yaw > 80.0f)
		{
			// 前進
			tempEnemyPosition.Y += m_MoveSpeed;
		}
		else if (m_pEnemy->GetActorRotation().Yaw > -10.0f)
		{
			// 前進
			tempEnemyPosition.X += m_MoveSpeed;
		}
		else
		{
			// 後退
			tempEnemyPosition.Y -= m_MoveSpeed;
		}


		// 修正後のアクター位置を設定
		m_pEnemy->SetActorLocation(tempEnemyPosition, false);


		if (m_IsAnimation)
		{
			m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
		}
	}
}

