// ばけものチーム
// ボスカッパのタワーの行動パターン

#include "ENEMY_TOWER_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TOWER_Component::UENEMY_TOWER_Component()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UENEMY_TOWER_Component::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントの取得
	m_pEnemy = Cast<AEnemyActor>(GetOwner());
	m_pJumpComponent     = Cast<UENEMY_TYPE_JUMP_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_JUMP_Component::StaticClass()));
	m_pStraightComponent = Cast<UENEMY_TYPE_STRAIGHT_Component>(m_pEnemy->GetComponentByClass(UENEMY_TYPE_STRAIGHT_Component::StaticClass()));
}


// Called every frame
void UENEMY_TOWER_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_pEnemy) 
	{
		if (m_pEnemy->GetActorLocation().Y < m_LimitPosY)
		{
			// 移動
			Move();
		}
		//else
		//{
		//	// プレイヤー近づいてきたらジャンプする
		//	CheckPlayer();
		//}

		//// 下に仲間のカッパがいる場合は一番下の個体じゃない
		//CheckEnemy();
	}
}

// 移動する-------------------------------------------------------------------------------------------------------------------------
void UENEMY_TOWER_Component::Move()
{
	if (m_pStraightComponent)
			m_pStraightComponent->EnemyMove();
}
//-------------------------------------------------------------------------------------------------------------------------

// ライントレースを利用して上下に何があるかを調べる-------------------------------------------------------------------------------------------------------------------------
void UENEMY_TOWER_Component::CheckLine(FVector _startPos, FVector _endPos, FName _targetTag,TFunctionRef<void()> _function)
{
	// 自分はあたらないようにする
	FCollisionQueryParams collisionParmas;
	collisionParmas.AddIgnoredActor(m_pEnemy);

	// 当たった対象を調べる
	TArray<FHitResult> outHit;
	bool isHit = GetWorld()->LineTraceMultiByChannel(outHit, _startPos, _endPos, ECC_WorldDynamic, collisionParmas);

	// 当たっていないなら返す
	if (!isHit)return;

	for (int n = 0; outHit.Num(); ++n)
	{
		//// プレイヤーが近づいたらジャンプする
		//if (outHit[n].GetActor()->ActorHasTag(_targetTag))
		//{
		//	_function();
		//}
	}
}
//-------------------------------------------------------------------------------------------------------------------------

// プレイヤー近づいてきたらジャンプする-------------------------------------------------------------------------------------------------------------------------
void UENEMY_TOWER_Component::CheckPlayer() 
{
	// プレイヤーが近くに来たらジャンプする
	TFunctionRef<void()> checkPlayer = [&]
	{
		if(m_pJumpComponent)
		m_pJumpComponent->m_IsJump = true;

		if(m_pEnemy)
		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
	};

	// ライントレーススタート地点
	FVector start = m_pEnemy->GetActorLocation();
	start.Y += +m_pEnemy->m_pEnemyMesh->Bounds.GetBox().GetSize().Y;

	// ライントレース終了地点
	FVector end = start;
	end.Z += m_LineLength;

	// ライントレースを用いて調べる
	CheckLine(start, end, "PlayerCharacter", checkPlayer);
}
//-------------------------------------------------------------------------------------------------------------------------

// 下に仲間のカッパがいる場合は一番下の個体じゃない-------------------------------------------------------------------------------------------------------------------------
void UENEMY_TOWER_Component::CheckEnemy()
{
	TFunctionRef<void()> checkEnemy = [&]
	{
		m_IsBase = false;
	};

	// ライントレーススタート地点
	FVector start = m_pEnemy->GetActorLocation();

	// ライントレース終了地点
	FVector end = start;
	end.Z -= m_LineLength;

	// ライントレースを用いて調べる
	CheckLine(start, end, "Enemy", checkEnemy);
}
//-------------------------------------------------------------------------------------------------------------------------
