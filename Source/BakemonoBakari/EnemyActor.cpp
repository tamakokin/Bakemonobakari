// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記
//			：2021/5/17 ジャンプする敵の行動プログラムを追加
//			：2021/5/23 消滅時の音を追加（伴野）
//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
//           
#include "EnemyActor.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CheckInScreen.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		// 当たり判定の無効化
		CollisionOff();
		// メッシュの無効化
		MeshOff();
		return;
	}
	else if ((m_EnemyState != ENEMY_STATE_DESTROY)&&(m_EnemyDamageCount <= 0))
	{
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
		}
	}
	m_pOverlappedActor = NULL;
}

// エネミーステータスコントロール
void AEnemyActor::EnemyStatusControl(float _deltaTime)
{
	ChangeAnim();
}

// ダメージ関数
void AEnemyActor::EnemyDamage()
{
	// ダメージ状態だったら
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY)) return;

	// 無敵状態なら
	if (m_EnemyDamageCount > 0)return;

	// 無敵状態にする
	m_EnemyDamageCount = 1;

	// 当たり判定の無効化
	CollisionOff();

	// ヒットエフェクト生成
	// アクター生成用
	FString BulletPath;
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor;
	// BPがあるパスを指定

	BulletPath = "/Game/Effects/Hit_EffectBP.Hit_EffectBP_C";
	// パス指定したBPのアクタークラスを格納
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

	if (subClass)
	{
		// 位置と回転を設定
		location = GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *m_pEnemyMesh->Bounds.BoxExtent.ToString());

		// エネミーにめり込まないよに前に出す
		location.X += 100.0f;
		rotation = FRotator(0.0f, 0.0f, 0.0f);

		// 設定した値を反映してスポーン
		actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);
	}

	// HPを減らす。ここはプレイヤーの攻撃値を参照するようにしてもいいかも
	--m_EnemyHP;
	//m_EnemyState = ENEMY_STATE_DAMAGE;
	ChangeAnim();

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//死亡エフェクト生成が入る
		//------------------------------------------------
		m_EnemyState = ENEMY_STATE_DESTROY;
		ChangeAnim();

		// スコアを加算する
		Cast<UMyGameInstance>(GetGameInstance())->AddScore(m_score, SCORE_TYPE::SCORE_TYPE_NORMAL_ENEMY);

		// 攻撃音を出す
		if (m_crashSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_crashSound, GetActorLocation());
		}
	}
}

// 無敵時のマテリアルの変化------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 無敵時間開始
void AEnemyActor::EnemyFlashing()
{
	int redCount = 5.0f;

	m_EnemyDamageCount++;

	if ((m_EnemyDamageCount % (redCount * 2)) == 0)
	{
		// マテリアル側の「Opacity」パラメータに数値を設定する
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	}
	else if ((m_EnemyDamageCount % redCount) == 0)
	{
		// マテリアル側の「Opacity」パラメータに数値を設定する
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.3f,0.0f,0.0f));
	}

	// 無敵時間の終了
	if (m_EnemyDamageCount >= 50) 
	{
		m_EnemyDamageCount = 0;
		// マテリアル側の「Opacity」パラメータに数値を設定する
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));

		// コライダーを復帰
		CollisionOn();
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 初期化
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);

	// マテリアル側の「Opacity」パラメータに数値を設定する
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	m_EnemyDamageCount = 0;
}

// 状態の初期化
// アニメーション切り替え
void AEnemyActor::ReSetState()
{
	m_EnemyState = ENEMY_STATE_IDLE;
}

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
	m_pEnemyMesh->SetVisibility(true);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// メッシュの非表示------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOff()
{
	m_pEnemyMesh->SetVisibility(false);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// アニメーション切り替え
void AEnemyActor::ChangeAnim()
{
	// 現在のアクターの状態を比較
	switch (m_EnemyState)
	{
	case AEnemyActor::ENEMY_STATE_NONE:
		break;

	case AEnemyActor::ENEMY_STATE_MOVE:
		UE_LOG(LogTemp, Warning, TEXT("Anime1"));
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
		UE_LOG(LogTemp, Warning, TEXT("Anime"));
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
