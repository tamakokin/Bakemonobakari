// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記

#include "EnemyActor.h"

// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_EnemyType(ENEMY_TYPE_NONE)
	, m_pOverlappedActor(NULL)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;

	// ベースに関連つけ
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("m_pBase"));
	// beseをrootに設定
	RootComponent = m_pBase;

	// メッシュを探す
	m_pEnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("m_pEnemyMesh"));
	// メッシュをつける
	m_pEnemyMesh->SetupAttachment(RootComponent);

	//	コリジョン判定用カプセルコンポーネント生成
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

}

// ゲームスタート時、または生成時に呼ばれる処理
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// 入力した敵の種類をプロパティに反映
	m_EnemyType = (ENEMY_TYPE)m_EnemyType_Any;
	
}

// 毎フレームの処理
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	if (m_pCapsuleComp != NULL)
	{
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
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
		if (m_pOverlappedActor->ActorHasTag("PlayerAttack"))
		{
			// ダメージを与える
			EnemyDamage();
		}

		// 接触したアクターのタグがプレイヤーであれば
		else if (m_pOverlappedActor->ActorHasTag("Player"))
		{
			// 動きを止める
			// moveStop();
		}
	}
	m_pOverlappedActor = NULL;
}


// 移動処理
void AEnemyActor::EnemyMove(float _deltaTime)
{
	// ダメージ状態だったら移動しない
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	FVector tempEnemyPosition = GetActorLocation();

	switch (m_EnemyType)
	{

	case AEnemyActor::ENEMY_TYPE_NONE:		// ダミー
		break;

	case AEnemyActor::ENEMY_TYPE_STOP:		// 動かない敵
		// 動かない
		break;

	case AEnemyActor::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵

		// エネミーの位置情報を取得
		tempEnemyPosition.X += m_moveSpeedX;
		// 修正後のアクター位置を設定
		SetActorLocation(tempEnemyPosition);

		//---------------------------------------------------------------
		// 段差があったら乗り越えるみたいなプログラムがあってもいいかも
		//---------------------------------------------------------------
		break;

	case AEnemyActor::ENEMY_TYPE_JUMP:		// ジャンプしてくる敵
		break;

	case AEnemyActor::ENEMY_TYPE_FRY:		// 空を飛ぶ敵
		//---------------------------------------------------------------
		// 今後によって書き換えます
		//---------------------------------------------------------------
		break;

	default:
		break;
	}
}

// 攻撃処理
void AEnemyActor::EnemyAttack()
{
	// ダメージ状態だったら攻撃しない
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	//---------------------------------------------------------------
	// 攻撃方法が不明のため条件分岐のみ記載します
	//---------------------------------------------------------------
	switch (m_EnemyType)
	{

	case AEnemyActor::ENEMY_TYPE_NONE:		// ダミー
		break;

	case AEnemyActor::ENEMY_TYPE_STOP:		// 動かない敵
		break;

	case AEnemyActor::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵
		break;

	case AEnemyActor::ENEMY_TYPE_JUMP:		// ジャンプしてくる敵
		break;

	case AEnemyActor::ENEMY_TYPE_FRY:		// 空を飛ぶ敵
		break;

	default:
		break;
	}
}

// ダメージ関数
void AEnemyActor::EnemyDamage()
{
	// ダメージ状態だったら
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	if (m_EnemyHP > 0)
	{
		//------------------------------------------------
		//hitエフェクト生成が入る
		//------------------------------------------------
		// HPを減らす。ここはプレイヤーの攻撃値を参照するようにしてもいいかも
		--m_EnemyHP;
	}

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//死亡エフェクト生成が入る
		//------------------------------------------------
		Destroy();
	}
}
