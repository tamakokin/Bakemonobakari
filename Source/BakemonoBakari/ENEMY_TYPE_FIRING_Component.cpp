// ばけものチーム
// 球を発射する敵

#include "ENEMY_TYPE_FIRING_Component.h"
#include "EnemyActor.h"
#include "Cucumber.h"
#include "Enemy_Rote_Component.h"
#include "Spawn.h"
#include "Engine.h"

// Sets default values for this component's properties
UENEMY_TYPE_FIRING_Component::UENEMY_TYPE_FIRING_Component():
m_AttackDelay(2.0f),
m_Speed(12.0f),
m_pEnemy(NULL),
m_pEnemyRote(NULL),
m_EnemyAttackingCount(0),
m_Spawn(NULL)
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

	m_Path = "/Game/Blueprints/MyCucumber.MyCucumber_C";

	// キュウリを生成するアクターを探索する
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	// キュウリを生成するアクターを取得する
	for (int i = 0; i < actors.Num(); i++)
	{
		if (Cast<ASpawn>(actors[i]))
		{
			if(actors[i]->ActorHasTag(FName(*m_Path)))
				m_Spawn = Cast<ASpawn>(actors[i]);
		}
	}

	// 存在しなかった場合、生成する
	if (!m_Spawn) 
	{
		// アクター生成用
		TSubclassOf<class AActor> subClass;
		FVector location;
		FRotator rotation;

		//パス指定したBPのアクタークラスを格納
		m_Spawn = GetWorld()->SpawnActor<ASpawn>();
		m_Spawn->SetUp("/Game/Blueprints/MyCucumber.MyCucumber_C", 3);

	}
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

	if (!m_pEnemy->GetIsAction())
	{
		m_EnemyAttackingCount = 0.0f;
		return;
	}

	// エネミーがダメージを受けた際にキュウリを消す
	if (!m_pEnemy->GetIsAction())
	{
		DestroyCucumber();
	}

	m_EnemyAttackingCount += DeltaTime;

	if (m_EnemyAttackingCount > m_AttackDelay)
	{
		if (m_pEnemyRote) 
		{
			// プレイヤーの方向を向く
			m_pEnemyRote->EnemyRote();
		}

		m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
	}
}

// キュウリを出す---------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_FIRING_Component::Spawn()
{
	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}

	if (!m_pEnemy->GetIsAction())
	{
		return;
	}

	// アクターを受け取る
	AActor* actor = m_Spawn->ReturnActor();

	// アクター表示する
	if (actor)
	{
		// アクターが見える状態にする
		actor->SetHidden(false);

		// 座標を初期化
		actor->SetActorLocation(m_pEnemy->GetActorLocation());

		// 手に付ける
		m_pCucumber = Cast<ACucumber>(actor);
		m_pCucumber->AttachToComponent(Cast< USceneComponent>(m_pEnemy->m_pEnemyMesh), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "Fire");

		actor = nullptr;
	}
}
// ---------------------------------------------------------------------------------------------------------------------
// キュウリを投げる---------------------------------------------------------------------------------------------------------------------
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
		m_pCucumber = NULL;
	}
}
//---------------------------------------------------------------------------------------------------------------------
// 攻撃アニメーションを終える---------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_FIRING_Component::AttackEnd()
{
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_IDLE);
	m_EnemyAttackingCount = 0.0f;
}
// -----------------------------------------------------------------------------------------------------------------------------------
// きゅりを消す---------------------------------------------------------------------------------------------------------------------
void UENEMY_TYPE_FIRING_Component::DestroyCucumber()
{
	if (m_pCucumber)
	{
		m_pCucumber->Des();
	}
}
//---------------------------------------------------------------------------------------------------------------------
