// ばけものチーム
// 球を発射する敵

#include "Kismet/GameplayStatics.h"
#include "ENEMY_TYPE_FIRING_Component.h"
#include "EnemyActor.h"
// Sets default values for this component's properties
UENEMY_TYPE_FIRING_Component::UENEMY_TYPE_FIRING_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UENEMY_TYPE_FIRING_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	// プレイヤーの探索
	SerchPlayer();
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

	// プレイヤーの探索
	if (!m_pPlayerActor)
	{
		SerchPlayer();
	}

	EnemyMove(DeltaTime);


	m_EnemyAttackingCount += DeltaTime;

	if (m_EnemyAttackingCount > m_AttackDelay)
	{
		EnemyAttack();
	}
}

void UENEMY_TYPE_FIRING_Component::EnemyMove(float _deltaTime)
{
	FVector tempEnemyPosition  = m_pEnemy->GetActorLocation();
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	FVector tempPlayerCharacterPosition = FVector::ZeroVector;
	if (m_pPlayerActor != NULL)
	{
		tempPlayerCharacterPosition = m_pPlayerActor->GetActorLocation();
	}


	if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
	{
		// 前向き
		tempEnemyRotation.Yaw = 90.f;
		m_StraightVector = false;
	}
	else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
	{
		// 後ろ向き
		tempEnemyRotation.Yaw = 270.f;
		m_StraightVector = true;
	}
	m_pEnemy->SetActorRotation(tempEnemyRotation);
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

	if (!m_StraightVector)
	{
		rotation.Yaw = 0.f;
	}
	else
	{
		rotation.Yaw = 180.f;
	}
	// 設定した値を反映してスポーン
	actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

	m_EnemyAttackingCount = 0.f;
	m_pEnemy->SetEnemyState(m_pEnemy->ENEMY_STATE_ATTACK);
}

void UENEMY_TYPE_FIRING_Component::SerchPlayer()
{
	// 一回だけ
	if (m_pPlayerActor == NULL)
	{
		// プレイヤーを探す
		TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

		if (actors.Num() > 0)
		{
			for (int idx = 0; idx < actors.Num(); idx++)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);

				if (pActor->ActorHasTag("PlayerCharacter"))
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Player"));
					m_pPlayerActor = pActor;
					break;
				}
			}
		}
	}
}

