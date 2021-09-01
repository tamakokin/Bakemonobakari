// ばけものチーム
// かまいたちの行動処理

#include "ENEMY_CHOP_START_Component.h"
#include "ENEMY_TYPE_JUMP_Component.h"
#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "Enemy_Rote_Component.h"
#include "DrawDebugHelpers.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_CHOP_START_Component::UENEMY_CHOP_START_Component() :
	m_LineLength(100.0f)
	, m_AttackStartTiming(100)
	, m_AttackEndTiming(300)
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UENEMY_CHOP_START_Component::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントの取得
	m_pEnemy = Cast<AEnemyActor>(GetOwner());
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
	m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
}
// Called every frame
void UENEMY_CHOP_START_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// コンポーネントが見つからない場合は捜す
	if ((!m_pStraightComponent) || (!m_pEnemy) || (!m_pEnemyRote))
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
		m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
		return;
	}

	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;

	// エネミーの行動処理
	EnemyMove(DeltaTime);
}

// プレイヤーに近づいて斬りつける-----------------------------------------------------------------------------------------------------------------------------------
void UENEMY_CHOP_START_Component::EnemyMove(float _deltaTime)
{
	if (!m_pEnemy)return;

	++m_AttackCount;

	// 攻撃を開始する時間を超えたら移動を始める
	if (m_AttackCount >= m_AttackStartTiming)
	{
		// 待機アニメーションの状態なら移動を開始する
		if (m_pEnemy->GetEnmeyState() == AEnemyActor::ENEMY_STATE_IDLE)
		{
			// プレイヤーの方を向く
			if (m_pEnemyRote)
			{
				m_pEnemyRote->EnemyRote();
			}

			// 移動アニメーションを開始する
			m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_MOVE);
		}
		else if (m_pEnemy->GetEnmeyState() == AEnemyActor::ENEMY_STATE_MOVE)
		{
			// 移動を開始する
			m_pStraightComponent->EnemyMove();

			// 移動方向に何があるか調べる
			CheckLine();
		}
	}

	if (!m_IsAttack)
	{
		// 攻撃時間が終了したら移動を止める
		if (m_AttackCount >= m_AttackEndTiming)
		{
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
	m_AttackCount = 0;

	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);
}
// --------------------------------------------------------------------------------------------------------------------------------------------------

// ライントレースを利用して前方あるアクターを判別して行動パターンを変える
void UENEMY_CHOP_START_Component::CheckLine()
{
	FVector start = m_pEnemy->GetActorLocation();
	FVector end = start;

	// エネミーの向いている方向に応じてラインを伸ばす方向を変える
	if (m_pEnemy->GetActorRotation().Yaw > 90)
	{
		end.Y -= m_LineLength;
	}
	else
	{
		end.Y += m_LineLength;
	}

	// 自分はあたらないようにする
	FCollisionQueryParams collisionParmas;
	collisionParmas.AddIgnoredActor(m_pEnemy);

	// 当たった対象を調べる
	TArray<FHitResult> outHit;
	bool isHit = GetWorld()->LineTraceMultiByChannel(outHit, start, end, ECC_WorldStatic, collisionParmas);

	// 当たっていないなら返す
	if (!isHit)return;

	// 壁に当たったかどうか
	bool isGround = false;

	for (int n = 0; n < outHit.Num();++n) 
	{
		if (outHit[n].GetComponent()->ComponentHasTag("Ground"))
		{
			isGround = true;
		}
		// 移動中にプレイヤーがあったら攻撃
		else if (outHit[n].GetActor()->ActorHasTag("PlayerCharacter"))
		{
			m_IsAttack = true;
			m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
			break;
		}
	}

	// プレイヤーに当たらず壁に当たった場合方向転換
	if (isGround)
	{
		if (m_pEnemyRote)
			m_pEnemyRote->TurnRote();
	}
}