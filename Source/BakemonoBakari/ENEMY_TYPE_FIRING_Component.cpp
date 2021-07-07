// ばけものチーム
// 球を発射する敵

#include "ENEMY_TYPE_FIRING_Component.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UENEMY_TYPE_FIRING_Component::UENEMY_TYPE_FIRING_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UENEMY_TYPE_FIRING_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

}

// Called every frame
void UENEMY_TYPE_FIRING_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}

	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)
	{
		m_EnemyAttackingCount = 0.0f;
		return;
	}

	// 画面外なら処理をしない
	if (!m_pEnemy->m_IsInScreen)return;

	m_EnemyAttackingCount += DeltaTime;

	if (m_EnemyAttackingCount > m_AttackDelay)
	{
		EnemyAttack();
	}
}


// 攻撃処理
void UENEMY_TYPE_FIRING_Component::EnemyAttack()
{
	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}

	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DAMAGE)
	{
		return;
	}

	// アクター生成用
	FString BulletPath;
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor;

	// BPがあるパスを指定
	BulletPath = "/Game/Blueprints/EnemyBulletBP.EnemyBulletBP_c";

	// パス指定したBPのアクタークラスを格納
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

	// 位置と回転を設定
	location = m_pEnemy->GetActorLocation(); //+ m_pCapsuleComp->GetRightVector() * 10;
	rotation = m_pEnemy->GetActorRotation();

	// 設定した値を反映してスポーン
	actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

	m_EnemyAttackingCount = 0.f;
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
}



