// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記
//			：2021/5/17 ジャンプする敵の行動プログラムを追加
//			：2021/5/23 消滅時の音を追加（伴野）
//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
//			：2021/8/17 倒しきったかどうかで与ダメージ音を切り替えるように（伴野）
//			：2021/9/ 7 倒した際にUIメッセージを出す 

#include "EnemyActor.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CheckInScreen.h"
#include "Enemy_Rote_Component.h"
#include "Materials/MaterialInstanceDynamic.h"

#define RAD_COLOR 0.3
#define DAMAGE_ANIME_END 30
#define FLASH_END 10
#define FLASH_TIMING 5
// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pPlayerCharacter(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_score(500.f)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_StartRote(FRotator::ZeroRotator)
	, m_DamageEndTiming(100)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;

	//	コリジョン判定用カプセルコンポーネント生成
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

	// メッシュを探す
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));


	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	// 画面何にいるかを判別するコンポーネントの生成
	m_pCheckInScreen = CreateDefaultSubobject<UCheckInScreen>(TEXT("CheckInScreen"));
	
	// 方向転換を行うコンポーネントの生成
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
}

// ゲームスタート時、または生成時に呼ばれる処理
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// コンポーネントの取得
	if (!m_pEnemyRote) 
	{
		m_pEnemyRote = Cast<UEnemy_Rote_Component>(GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
	}


	// 初期位置の取得
	m_initEnemyPosition = GetActorLocation();

	// 初期回転の取得
	m_StartRote = GetActorRotation();

	// HPの初期化
	m_EnemyHP = m_EnemyHPMax;

	// 行動可能状態にする
	m_IsAction = true;

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	if (m_pEnemyMesh)
	{
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else
	{
		// メッシュを探す
		m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
}

// 毎フレームの処理 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		// 当たり判定の無効化
		CollisionOff();
		// メッシュの無効化
		MeshOff();

		// 行動できないようにする
		m_IsAction = false;
		return;
	}
	else if ((m_EnemyState != ENEMY_STATE_DESTROY) && (m_EnemyDamageCount <= 0))
	{
		// 行動可能状態にする
		m_IsAction = true;
		m_IsInScreen = true;
		MeshOn();
		CollisionOn();
	}

	// 無敵時間なら点滅させる
	if (m_EnemyDamageCount > 0)
	{
		EnemyFlashing();
	}

	// ステータスの更新
	ChangeAnim();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// オーバーラップ関数----------------------------------------------------------------------------------------------------------------------------------------------------
UFUNCTION() void AEnemyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// オーバーラップしたアクターを格納
	m_pOverlappedActor = OtherActor;

	// アクターのポインターがぬるでなければ
	if (m_pOverlappedActor != NULL)
	{
		// 接触したアクターのタグがプレイヤーの攻撃であれば
		if (OtherComp->ComponentHasTag("PlayerAttack"))
		{
			// ダメージを与える
			EnemyDamage();
		}
		// 穴に落ちると死亡
		else if (m_pOverlappedActor->ActorHasTag("Hole"))
		{
			m_EnemyState = ENEMY_STATE_DESTROY;
		}
	}
	m_pOverlappedActor = NULL;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ダメージ関数-------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::EnemyDamage()
{
	// 無敵状態なら
	if (m_EnemyDamageCount > 0)return;

	// 行動不能状態にする
	m_IsAction = false;

	// 無敵状態にする
	m_EnemyDamageCount = 1;

	// ダメージを受けた状態にする
	m_EnemyState = ENEMY_STATE_DAMAGE;

	// 赤色にする
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(RAD_COLOR, 0.0f, 0.0f));

	// ヒットエフェクトを出す
	Hit();

	// HPを減らす。ここはプレイヤーの攻撃値を参照するようにしてもいいかも
	--m_EnemyHP;


	if (m_EnemyHP <= 0)
	{
		Des();
	}
	else
	{
		// 与ダメージ音を出す
		if (m_EnemyDamageSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_EnemyDamageSound, GetActorLocation());
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 死亡処理------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::Des() 
{
	if (!m_pEnemyRote)return;

	// 死亡アニメーション再生
	m_EnemyState = ENEMY_STATE_DESTROY;
	ChangeAnim();

	// 行動不能にする
	m_IsAction = false;

	// プレイヤーの方向を向く
	m_pEnemyRote->EnemyRote();

	// 見えるようにする　
	m_pEnemyMesh->SetVisibility(true);

	// スコアを加算する
	Cast<UMyGameInstance>(GetGameInstance())->AddScore(m_score, SCORE_TYPE::SCORE_TYPE_NORMAL_ENEMY);
	
	// スコアUIを出現させる
	//if (m_pEnemyScore != NULL)
	{
		AEnemyScore* m_pEnemyScore;
		FString ScorePath = "/Game/Blueprints/EnemyScore.EnemyScore_C";
		TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(ScorePath)).LoadSynchronous();

		m_pEnemyScore = GetWorld()->SpawnActor<AEnemyScore>(subClass, GetActorLocation(), FRotator(0.f, 180.f, 0.f));
		m_pEnemyScore->SetScore(m_score);
	}

	// 倒しきった音を出す
	if (m_EnemyLethalDamageSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_EnemyLethalDamageSound, GetActorLocation());
	}

	// 当たり判定の無効化
	CollisionOff();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 無敵時のマテリアルの変化------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 無敵時間開始
void AEnemyActor::EnemyFlashing()
{
	m_EnemyDamageCount++;

	// 赤色をもとに戻す
	if (m_EnemyDamageCount >= FLASH_END)
	{
		// マテリアル側の「Opacity」パラメータに数値を設定する
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	}

	if (m_EnemyState != ENEMY_STATE_DESTROY)
	{
		// ダメージアニメーションを終了
		if (m_EnemyDamageCount == DAMAGE_ANIME_END)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_IsAction = true;
		}

		// 無敵時には点滅させる
		if (m_EnemyDamageCount % (FLASH_TIMING *2) == 0)
		{
			m_pEnemyMesh->SetVisibility(false);
		}
		else if (m_EnemyDamageCount % FLASH_TIMING == 0)
		{
			m_pEnemyMesh->SetVisibility(true);
		}

		// 無敵時間の終了
		if (m_EnemyDamageCount > m_DamageEndTiming)
		{
			m_EnemyDamageCount = 0;

			m_pEnemyMesh->SetVisibility(true);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 初期化----------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);

	// マテリアル側の「Opacity」パラメータに数値を設定する
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	m_EnemyDamageCount = 0;

	CollisionOn();
	MeshOn();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 状態の初期化
// アニメーション切り替え ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::ReSetState()
{
	m_EnemyState = ENEMY_STATE_IDLE;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 当たり判定の無効化------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::CollisionOff()
{
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pEnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 当たり判定の有効化------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::CollisionOn()
{
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY))return;
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_pEnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// メッシュの表示------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOn()
{
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY))return;
	SetActorHiddenInGame(false);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// メッシュの非表示------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOff()
{
	SetActorHiddenInGame(true);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// アニメーション切り替え------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::ChangeAnim()
{
	// 現在のアクターの状態を比較
	switch (m_EnemyState)
	{
	case AEnemyActor::ENEMY_STATE_NONE:
		break;

	case AEnemyActor::ENEMY_STATE_MOVE:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = true;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_IDLE:
		m_bIdling = true;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_DAMAGE:
		m_bIdling = false;
		m_bDamage = true;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_DESTROY:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = true;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_STOP:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = true;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_ATTACK:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = true;
		break;
	default:
		break;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
