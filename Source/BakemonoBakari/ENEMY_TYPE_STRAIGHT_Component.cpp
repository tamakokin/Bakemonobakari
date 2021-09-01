// ばけものチーム
// 真っ直ぐ突っ込んで来る敵の行動処理

#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_STRAIGHT_Component::UENEMY_TYPE_STRAIGHT_Component() 

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
	if (!m_pEnemy)return;
	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;

	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	// 回転をカウント
	if (m_IsTurn)
	{
	
		// 可動範囲を超えたら
		if ((tempEnemyPosition.Y > m_MaxMoveY) || (tempEnemyPosition.Y < m_MinMoveY))
		{
			
			// カウントする
			m_RoteCount++;
		}
	}
	// 回転できるようになるまでをカウント
	else 
	{
		m_TurnCount++;

		// 回転をできるようにする
		if (m_TurnCount > m_TurnTiming)
		{
			m_TurnCount = 0;
			m_IsTurn = true;
		}
	}

	// エネミーを180度回転させる
	if (m_RoteCount > 0)
	{
		tempEnemyRotation.Yaw += m_RoteSpeed;

		// 回転が270を超えないようにする
		if (tempEnemyRotation.Yaw >= 270)
		{
			tempEnemyRotation.Yaw = 90.0f;
		}

		m_pEnemy->SetActorRotation(FRotator(tempEnemyRotation.Pitch, tempEnemyRotation.Yaw, tempEnemyRotation.Roll));

		// 180ド回転したらカウントを0にする
		if (m_RoteSpeed * m_RoteCount >= 180)
		{
			m_RoteCount = 0;

			m_IsTurn = false;
		}
	}
	// 移動を行う
	else
	{
		// 進む方向に応じて
		if (tempEnemyRotation.Yaw >= 170.0f)
		{
			// 前進
			tempEnemyPosition.X -= m_moveSpeedY;
		}
		else if (tempEnemyRotation.Yaw > 80.0f) 
		{
			// 前進
			tempEnemyPosition.Y += m_moveSpeedY;
		}
		else if (tempEnemyRotation.Yaw > -10.0f) 
		{
			// 前進
			tempEnemyPosition.X += m_moveSpeedY;
		}
		else
		{
			// 後退
			tempEnemyPosition.Y -= m_moveSpeedY;
		}


		// 修正後のアクター位置を設定
		m_pEnemy->SetActorLocation(tempEnemyPosition, false);


		if (m_IsAnimation)
		{
			m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
		}
	}
}

