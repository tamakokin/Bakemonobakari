// ばけものチーム
// 球を発射する敵

#include "ENEMY_TYPE_FIRING_Component.h"
#include "EnemyActor.h"
#include "Cucumber.h"
#include "Enemy_Rote_Component.h"

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
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(m_pEnemy->GetComponentByClass(UEnemy_Rote_Component::StaticClass()));

}

// Called every frame
void UENEMY_TYPE_FIRING_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 死亡時に処理差させない
	if (m_pEnemy->GetEnmeyState() == m_pEnemy->ENEMY_STATE_DESTROY)return;
	if (m_pEnemy->m_pCapsuleComp->GetCollisionEnabled() == AACF_NONE)return;

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
		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);

		if (m_pEnemyRote) 
		{
			// プレイヤーの方向を向く
			m_pEnemyRote->EnemyRote();
		}
	}
}

// キュウリを出す
void UENEMY_TYPE_FIRING_Component::Spawn()
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

	BulletPath = "/Game/Blueprints/MyCucumber.MyCucumber_C";

	// パス指定したBPのアクタークラスを格納
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

	// 位置と回転を設定
	location = m_pEnemy->GetActorLocation(); //+ m_pCapsuleComp->GetRightVector() * 10;
	rotation = m_pEnemy->GetActorRotation();

	// 設定した値を反映してスポーン
	actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

	actor->AttachToComponent(m_pEnemy->m_pEnemyMesh,FAttachmentTransformRules::KeepWorldTransform,"Fire");

	m_pCucumber = Cast<ACucumber>(actor);
}

// キュウリを投げる
void UENEMY_TYPE_FIRING_Component::EnemyAttack() 
{
	if (m_pCucumber)
	{
		if (m_pEnemy->GetActorRotation().Yaw < -80.0f)
		{
			m_pCucumber->MoveStart(m_Speed,true);
		}
		else 
		{
			m_pCucumber->MoveStart(m_Speed, false);
		}
	}
}

// 攻撃を終える
void UENEMY_TYPE_FIRING_Component::AttackEnd()
{
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);
	m_pEnemy->ChangeAnim();
	m_EnemyAttackingCount = 0.0f;
}

