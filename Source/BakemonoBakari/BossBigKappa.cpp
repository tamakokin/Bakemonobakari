// ばけものチーム
// Stage1のボスカッパの行動パターン

#include "BossBigKappa.h"
#include "Engine.h"
#include "Shirikodama.h"
#include "EnemyActor.h"

#define KAPPA 0
#define SHIRIKODAMA 1
#define TOWERKAPPA 2

#define KAPPASIZE 70

// Sets default values
ABossBigKappa::ABossBigKappa():
	m_Phase(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABossBigKappa::BeginPlay()
{
	Super::BeginPlay();

	// 行動可能状態にする
	//m_IsAction = true;

	// ロード終わる前に行動不可能状態にする
	m_IsAction_bp = false;
	m_IsAction = false;

	// メッシュを探す
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	m_pCapsuleComp = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));

	// プレイヤーを捜す
	SerchPlayer();

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	if (m_pCapsuleComp)
	{
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABossBigKappa::OnOverlapBegin);
	}
	else
	{
		// メッシュを探す
		m_pCapsuleComp = Cast<UCapsuleComponent>(GetComponentByClass(UCapsuleComponent::StaticClass()));
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABossBigKappa::OnOverlapBegin);
	}

	// アクターを生成する座標を設定
	m_SpawnPos += GetActorLocation();

	// アクターを生成するアクターを準備する
	SpawnSetUp();

	// タワーカッパを呼び出す
	SpawnTowerKappa();
}

// Called every frame
void ABossBigKappa::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ロード終わてから行動可能状態にする
	if (m_IsAction_bp == true)
	{
		m_IsAction = m_IsAction_bp;
	}

	// 死亡時には処理しない
	if (m_BossState == BOSS_STATE::BOSS_STATE_DESTROY)return;

	if (m_IsAction)
	{
		// カウントを取る
		m_AttackCount++;

		switch (m_Phase)
		{
			case 0:
			{
				Pattern1();
				break;
			}
			case 1:
			{
				Pattern2();
				break;
			}
			case 2:
			{
				Pattern3();
				break;
			}
		}
	}
	else 
	{
		if (m_pPlayerActor->GetActorLocation().Y < m_KnockbackPosY)
		{
			FVector playerPos = m_pPlayerActor->GetActorLocation();
			playerPos.Y += m_KnockbackSpeed;

			m_pPlayerActor->SetActorLocation(playerPos);
		}
	}

	// アニメーション更新
	ChangeAnim();
}

// フェーズ1の行動パターン----------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::Pattern1()
{
	int shirikodamaTaiming = m_AttackTiming[m_Phase];
	int kappaTaiming = m_KappaTiming[m_Phase];

	if (m_AttackCount == shirikodamaTaiming)
	{
		m_BossState = BOSS_STATE::BOSS_STATE_ATTACK;
	}
	else if (m_AttackCount == kappaTaiming)
	{
		SpawnKappa(FMath::RandRange(1,2));
		m_AttackCount = 0;
	}
}
// ------------------------------------------------------------------------------------------------------------------------

// フェーズ2の行動パターン----------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::Pattern2()
{
	int shirikodamaTaiming = m_AttackTiming[m_Phase];
	int kappaTaiming = m_KappaTiming[m_Phase];

	if (m_AttackCount == shirikodamaTaiming)
	{
		m_BossState = BOSS_STATE::BOSS_STATE_ATTACK;
	}
	else if (m_AttackCount == kappaTaiming)
	{
		SpawnKappa(FMath::RandRange(1, 3));
		m_AttackCount = 0;
	}
}
// ------------------------------------------------------------------------------------------------------------------------

// フェーズ3の行動パターン----------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::Pattern3()
{
	int shirikodamaTaiming = m_AttackTiming[m_Phase];
	int kappaTaiming = m_KappaTiming[m_Phase];

	if (m_AttackCount == shirikodamaTaiming)
	{
		m_BossState = BOSS_STATE::BOSS_STATE_ATTACK;
	}
	else if (m_AttackCount == kappaTaiming)
	{
		SpawnKappa(FMath::RandRange(2, 4));
		m_AttackCount = 0;
	}
}
// ------------------------------------------------------------------------------------------------------------------------

// アクターを生成するアクターを準備------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::SpawnSetUp()
{
	for (int n = 0; n < 3; ++n)
	{
		//パス指定したBPのアクタークラスを格納
		m_Spawn.Add(GetWorld()->SpawnActor<ASpawn>());
	}
	m_Spawn[KAPPA]->SetUp("/Game/Blueprints/TibiKappaDashBP.TibiKappaDashBP_C", 5);
	m_Spawn[SHIRIKODAMA]->SetUp("/Game/Blueprints/ShirikodamaBP.ShirikodamaBP_C", 5);
	m_Spawn[TOWERKAPPA]->SetUp("/Game/Blueprints/TibiKappaTowerBP.TibiKappaTowerBP_C", 20);
}
// ------------------------------------------------------------------------------------------------------------------------

// プレイヤーの探索を行う------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::SerchPlayer()
{
	// 一回だけ
	if (m_pPlayerActor)return;

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
				m_pPlayerActor = pActor;
				break;
			}
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------

// 突撃するカッパを呼び出す--------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::SpawnKappa(int _num)
{
	for (int n = 0; n < _num; ++n)
	{
		// アクターを受け取る
		AActor* actor = m_Spawn[KAPPA]->ReturnActor();

		// アクター表示する
		if (actor)
		{
			AEnemyActor* pEnemy = Cast<AEnemyActor>(actor);

			if (pEnemy)
			{
				// アクターが見える状態にする
				pEnemy->ReStartPosition();
			}
			// 座標を初期化
			FVector spawnPos = m_SpawnPos;
			spawnPos.Z += (KAPPASIZE * n);
			actor->SetActorLocation(spawnPos);

			actor->SetActorRotation(FRotator(0.0f, 90.0f, 00.0f));
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------
// 尻子玉を呼び出す--------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::SpawnShirikodama()
{
	// アクターを受け取る
	AActor* actor = m_Spawn[SHIRIKODAMA]->ReturnActor();

	// アクター表示する
	if (actor)
	{// アクター消す
		if (actor)
		{
			AShirikodama* pShirikodama = Cast<AShirikodama>(actor);

			if (pShirikodama)
			{
				// エネミーの死亡処理
				pShirikodama->MoveStart(FMath::RandRange(4.0f,6.0f), FMath::RandRange(18.0f, 22.0f));
			}
		}

		// 座標を初期化
		FVector spawnPos = m_pEnemyMesh->GetSocketLocation("Spawn");
		spawnPos.X = 0.0f;

		actor->SetActorLocation(spawnPos);
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------
// タワーカッパを呼び出す--------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::SpawnTowerKappa()
{
	for (int n = 0; n < m_KappaCount[m_Phase]; ++n)
	{
		// アクターを受け取る
		AActor* actor = m_Spawn[TOWERKAPPA]->ReturnActor();

		// アクター表示する
		if (actor)
		{
			AEnemyActor* pEnemy = Cast<AEnemyActor>(actor);

			if (pEnemy)
			{
				// アクターが見える状態にする
				pEnemy->ReStartPosition();
			}

			// 一個ずつ積み上げいく
			FVector spawnPos = m_SpawnPos;

			// カッパ分の高さを加算
			spawnPos.Z += (KAPPASIZE * n);
			// 座標を初期化
			actor->SetActorLocation(spawnPos);

			actor->SetActorRotation(FRotator(0.0f, 90.0f, 00.0f));
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------
// オーバーラップ関数----------------------------------------------------------------------------------------------------------------------------------------------------
UFUNCTION() void ABossBigKappa::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// アクターのポインターがぬるでなければ
	if (OtherActor != NULL)
	{
		// 接触したアクターのタグがプレイヤーの攻撃であれば
		if (OtherComp->ComponentHasTag("PlayerAttack"))
		{
			// ダメージを与える
			EnemyDamage();
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ダメージ関数-------------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::EnemyDamage()
{
	// ダメージ状態なら処理しない
	if (m_BossState == BOSS_STATE::BOSS_STATE_DAMAGE)return;

	// ダメージ状態にする
	m_BossState = BOSS_STATE::BOSS_STATE_DAMAGE;

	// 当たり判定をなくす
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	KillEnemy();

	// アニメーション更新
	ChangeAnim();

	// 行動不能状態にする
	m_IsAction = false;

	// 赤色にする
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.3f, 0.0f, 0.0f));

	// 攻撃のカウントをリセット
	m_AttackCount = 0;

	// フェーズを進める
	m_Phase++;
	
	// フェーズの数を超えたら死亡
	if (m_Phase >= 3)
	{
		// ダメージ状態にする
		m_BossState = BOSS_STATE::BOSS_STATE_DESTROY;

		// アニメーション更新
		ChangeAnim();

		// 死亡処理を呼び出す
		Des();
	}
	else 
	{
		// タワーカッパを出す
		SpawnTowerKappa();
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成したエネミーを消す------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::KillEnemy()
{
	// タワーカッパを消す
	for (int n = 0; n < m_Spawn[TOWERKAPPA]->m_SpawnActor.Num(); ++n)
	{
		// アクターを受け取る
		AActor* actor = m_Spawn[TOWERKAPPA]->m_SpawnActor[n];

		if (actor)
		{
			AEnemyActor* pEnemy = Cast<AEnemyActor>(actor);

			if (pEnemy)
			{
				// エネミーの死亡処理
				pEnemy->Des();
			}
		}
	}
	// 突撃するカッパを消す
	for (int n = 0; n < m_Spawn[KAPPA]->m_SpawnActor.Num(); ++n)
	{
		// アクターを受け取る
		AActor* actor = m_Spawn[KAPPA]->m_SpawnActor[n];

		// アクター消す
		if (actor)
		{
			AEnemyActor* pEnemy = Cast<AEnemyActor>(actor);

			if (pEnemy)
			{
				// エネミーの死亡処理
				pEnemy->Des();
			}
		}
	}

	// 尻子玉を消す
	for (int n = 0; n < m_Spawn[SHIRIKODAMA]->m_SpawnActor.Num(); ++n)
	{
		// アクターを受け取る
		AActor* actor = m_Spawn[SHIRIKODAMA]->m_SpawnActor[n];

		// アクター消す
		if (actor)
		{
			AShirikodama* pShirikodama = Cast<AShirikodama>(actor);

			if (pShirikodama)
			{
				// エネミーの死亡処理
				pShirikodama->Des();
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 状態の初期化
// アニメーション切り替え--------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::ReSetState()
{
	// 色をもとに戻す
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));

	// 行動可能状態にする
	m_IsAction = true;

	// アニメーションを待機状態に
	m_BossState = BOSS_STATE_IDLE;

	// アニメーション更新
	ChangeAnim();

	// 当たり判定を戻す
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
// --------------------------------------------------------------------------------------------------------------------------------------------

// アニメーション切り替え------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ABossBigKappa::ChangeAnim()
{
	// 現在のアクターの状態を比較
	switch (m_BossState)
	{
	case ABossBigKappa::BOSS_STATE_NONE:
		break;

	case ABossBigKappa::BOSS_STATE_IDLE:
		m_IsIdling = true;
		m_IsDamage = false;
		m_IsDestroy = false;
		m_IsAttacking = false;
		break;

	case ABossBigKappa::BOSS_STATE_DAMAGE:
		m_IsIdling = false;
		m_IsDamage = true;
		m_IsDestroy = false;
		m_IsAttacking = false;
		break;

	case ABossBigKappa::BOSS_STATE_DESTROY:
		m_IsIdling = false;
		m_IsDamage = false;
		m_IsDestroy = true;
		m_IsAttacking = false;
		break;

	case ABossBigKappa::BOSS_STATE_ATTACK:
		m_IsIdling = false;
		m_IsDamage = false;
		m_IsDestroy = false;
		m_IsAttacking = true;
		break;
	default:
		break;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
