// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記
//			：2021/5/17 ジャンプする敵の行動プログラムを追加
//			：2021/5/23 消滅時の音を追加（伴野）
//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
//           
#include "Kismet/GameplayStatics.h"
#include "EnemyActor.h"
#include "CheckInScreen.h"

// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pPlayerCharacter(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_StartRote(FRotator::ZeroRotator)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;

	// ベースに関連つけ
	//m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("m_pBase"));
	// beseをrootに設定
	//RootComponent = m_pBase;

	//	コリジョン判定用カプセルコンポーネント生成
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

	// メッシュを探す
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	// 画面何にいるかを判別するコンポーネントの生成
	m_pCheckInScreen = CreateDefaultSubobject<UCheckInScreen>(TEXT("CheckInScreen"));

	//m_pCheckInScreen = Cast<UCheckInScreen>(this);
}

// ゲームスタート時、または生成時に呼ばれる処理
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// 初期位置の取得
	m_initEnemyPosition = GetActorLocation();

	// 初期回転の取得
	m_StartRote = GetActorRotation();

	// HPの初期化
	m_EnemyHP = m_EnemyHPMax;
}

// 毎フレームの処理
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	if (m_pEnemyMesh)
	{
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else 
	{
		// メッシュを探す
		m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	}

	// 死んでるのなら処理しない
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		Des();
		return;
	}

	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		Des();
		return;
	}
	else if (m_EnemyState != ENEMY_STATE_DESTROY)
	{
		m_IsInScreen = true;
		Indication();
	}

	// ステータスの更新
	EnemyStatusControl(DeltaTime);
}

// オーバーラップ関数
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

		// 接触したアクターのタグがプレイヤーであれば
		else if (m_pOverlappedActor->ActorHasTag("PlayerCharacter"))
		{
			//// 動きを止める
			//EnemyStop();
		}
		// 穴に落ちると死亡
		else if (m_pOverlappedActor->ActorHasTag("Hole"))
		{
			m_EnemyState = ENEMY_STATE_DESTROY;
			Des();
		}
	}
	m_pOverlappedActor = NULL;
}

// エネミーステータスコントロール
void AEnemyActor::EnemyStatusControl(float _deltaTime)
{
	// 死亡時
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		// 死亡アニメーション時間カウント
	}
	// 硬直状態の時
	else if (m_EnemyState == ENEMY_STATE_STOP)
	{
		// 硬直アニメーション時間カウント
	}
	// ダメージ状態
	else if (m_EnemyState == ENEMY_STATE_DAMAGE)
	{
		// ダメージアニメーションカウント
		m_EnemyDamageAnimationCount += _deltaTime;

		if (m_EnemyDamageAnimationCount > m_DamageAnimationTime)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_EnemyDamageAnimationCount = 0.f;
			ChangeAnim();
		}
	}
	else if (m_EnemyState == ENEMY_STATE_ATTACK)
	{
		// 攻撃カウント
		m_EnemyAttackAnimationCount += _deltaTime;

		if (m_EnemyAttackAnimationCount > m_AttackAnimationTime)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_EnemyAttackAnimationCount = 0.f;
			ChangeAnim();
		}
	}
}

// ダメージ関数
void AEnemyActor::EnemyDamage()
{
	// ダメージ状態だったら
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY)) return;

	m_EnemyState = ENEMY_STATE_DAMAGE;
	EnemyDamageEvent();

	//------------------------------------------------
	//hitエフェクト生成が入る
	//------------------------------------------------
	// HPを減らす。ここはプレイヤーの攻撃値を参照するようにしてもいいかも
	--m_EnemyHP;
	m_EnemyState = ENEMY_STATE_DAMAGE;
	ChangeAnim();

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//死亡エフェクト生成が入る
		//------------------------------------------------
		m_EnemyState = ENEMY_STATE_DESTROY;
		ChangeAnim();

		// 攻撃音を出す
		if (m_crashSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_crashSound, GetActorLocation());
		}
		//Des();
	}
}

// 初期化
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);
}

// アニメーション切り替え
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
