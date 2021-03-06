// ばけものチーム
#include "ENEMY_TYPE_JUMP_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_JUMP_Component::UENEMY_TYPE_JUMP_Component() :
	m_JumpWait(0.0f),
	m_IsGravity(false),
	m_IsJump(false),
	m_InitVelocityZ(5.0f),
	m_JumpGravity(9.0f),
	m_pEnemy(NULL),
	m_EnemyGravityDeltaTime(0.0f),
	m_StraightVector(false),
	m_SwitchVector(false),
	m_IsAir(false),
	m_IsHit(false)
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UENEMY_TYPE_JUMP_Component::BeginPlay()
{
	Super::BeginPlay();
	m_pEnemy = Cast<AEnemyActor>(GetOwner());
}

// Called every frame
void UENEMY_TYPE_JUMP_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// エネミーが行動不能なら処理をしない
	if (!m_pEnemy->GetIsAction())return;

	if (m_pEnemy->m_pEnemyMesh)
	{
		m_pEnemy->m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::BeginHit);
		m_pEnemy->m_pEnemyMesh->OnComponentEndOverlap.AddDynamic(this, &UENEMY_TYPE_JUMP_Component::EndHit);
	}

	// 重力の移動のみを行う
	if (m_IsGravity) 
	{
		FVector tempPos = FVector::ZeroVector;

		tempPos += Gravity(DeltaTime);

		// エネミーの座標を変える
		m_pEnemy->SetActorLocation(tempPos + m_pEnemy->GetActorLocation(), true);
	}

	if (m_IsJump)
	{
		if (!m_pEnemy->GetIsAction())
		{
			m_IsJump = false;
			return;
		}

		EnemyMove(DeltaTime);
	}
}

// エネミーの上下の移動を行う-------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::EnemyMove(float _deltaTime)
{
	FVector tempPos = FVector::ZeroVector;

	// 待機時間を超えるとジャンプ開始
	tempPos += Jump(_deltaTime);
	tempPos += Gravity(_deltaTime);

	//// エネミーの座標を変える
	m_pEnemy->SetActorLocation(tempPos + m_pEnemy->GetActorLocation(), true);
}
//----------------------------------------------------------------------------------------------------------------------------------------

// ジャンプして上方向に移動する----------------------------------------------------------------------------------------------------------------------------------------
FVector UENEMY_TYPE_JUMP_Component::Jump(float _deltaTime)
{
	if (!m_pEnemy->GetIsAction())return FVector::ZeroVector;

	// 一時的に保持する座標
	FVector tempEnemyPosition = FVector::ZeroVector;
	tempEnemyPosition.Z += m_InitVelocityZ;
	return tempEnemyPosition;
}
//----------------------------------------------------------------------------------------------------------------------------------------

// 重力による下方向の移動//----------------------------------------------------------------------------------------------------------------------------------------
FVector UENEMY_TYPE_JUMP_Component::Gravity(float _deltaTime)
{
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return FVector::ZeroVector;
	if (m_pEnemy->m_pCapsuleComp->GetCollisionEnabled() == AACF_NONE)return FVector::ZeroVector;

	// 一時的に保持する座標
	FVector tempEnemyPosition = FVector::ZeroVector;

	// 鉛直投げ上げ
	float tempDeltaZ = 0.0f;

	// 時間をカウント
	m_EnemyGravityDeltaTime += _deltaTime;

	// 重力移動
	tempDeltaZ = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));
	tempEnemyPosition.Z += tempDeltaZ;

	// 値の反映
	return tempEnemyPosition;
}
//----------------------------------------------------------------------------------------------------------------------------------------

// 地面に降りたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	// タグで判定
	if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Ground")) || (OtherActor->ActorHasTag("Enemy")))
	{
		float gravity = -0.5 * m_JumpGravity * ((m_EnemyGravityDeltaTime) * (m_EnemyGravityDeltaTime));

		FVector cenetr;
		FVector meshSize;
		m_pEnemy->GetActorBounds(true, cenetr, meshSize);

		if ((CheckLine(FVector(-meshSize.X,-meshSize.Y,0.0f))|| CheckLine(FVector(meshSize.X, meshSize.Y,0.0f))))
		{
			if (FMath::Abs(gravity) > m_InitVelocityZ)
			{
				m_EnemyGravityDeltaTime = 0.0f;
				m_IsAir = false;

				if (m_IsAnimation)
				{
					// エネミーの状態を変える
					m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);
					m_IsJump = false;
				}
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 地面から離れたときの処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_JUMP_Component::EndHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 接触した際の処理
	if (!OtherActor || (OtherActor == m_pEnemy) || (!OtherComp))return;

	if ((m_pEnemy->GetActorLocation().Z > OtherActor->GetActorLocation().Z))
	{
		// タグで判定
		if ((OtherActor->ActorHasTag("Ground")) || (OverlappedComponent->ComponentHasTag("Block")) || (OtherActor->ActorHasTag("Enemy")))
		{
			m_IsAir = true;
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ライントレースを利用して前方あるアクターを判別して行動パターンを変える--------------------------------------------------------------------------------
bool UENEMY_TYPE_JUMP_Component::CheckLine(FVector _MeshSize)
{

	FVector start = m_pEnemy->GetActorLocation() + _MeshSize;
	FVector end = start;

	// エネミーの向いている方向に応じてラインを伸ばす方向を変える
	end.Z -= 100;

	// 自分はあたらないようにする
	FCollisionQueryParams collisionParmas;
	collisionParmas.AddIgnoredActor(m_pEnemy);

	// 当たった対象を調べる
	TArray<FHitResult> outHit;
	bool isHit = GetWorld()->LineTraceMultiByChannel(outHit, start, end, ECC_WorldStatic, collisionParmas);

	// 当たっていないなら返す
	if (!isHit)return false;

	for (int n = 0; n < outHit.Num(); ++n)
	{
		if (outHit[n].GetComponent()->ComponentHasTag("Ground"))
		{
			return true;
		}
	}
	return false;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------
