// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記
//			：2021/5/17 ジャンプする敵の行動プログラムを追加
//			：2021/5/23 消滅時の音を追加（伴野）
//			：2021/5/29 画面外にいる場合は動かないようにする（大金）

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
	if (m_pCapsuleComp != NULL)
	{
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	// 死んでるのなら処理しない
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		return;
	}
	//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
	if (!m_pCheckInScreen->Check(GetActorLocation())) 
	{ 
		Des();
		return; 
	}
	else 
	{
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


// 移動処理
void AEnemyActor::EnemyMove(float _deltaTime)
{
	// ダメージ状態だったら移動しない
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	FVector tempEnemyPosition = GetActorLocation();
	FRotator tempEnemyRotation = GetActorRotation();

	ChangeAnim();

	//switch (m_EnemyType)
	//{

	//case ENEMY_TYPE::ENEMY_TYPE_NONE:		// ダミー
	//	break;

	//case ENEMY_TYPE::ENEMY_TYPE_STOP:		// 動かない敵
	//	// 動かない
	//	break;

	////case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵

	////	// 方向転換
	////	if (m_SwitchVector)
	////	{
	////		if (!m_StraightVector)
	////		{
	////			// 前向き
	////			tempEnemyRotation.Yaw = 90.f;
	////		}
	////		else
	////		{
	////			// 後ろ向き
	////			tempEnemyRotation.Yaw = 270.f;
	////		}

	////		m_SwitchVector = false;
	////		SetActorRotation(tempEnemyRotation);
	////	}

	////	m_prevEnemyPosition = tempEnemyPosition;

	////	// 進む方向に応じて
	////	if (!m_StraightVector)
	////	{
	////		// 前進
	////		tempEnemyPosition.Y += m_moveSpeedY;
	////	}
	////	else
	////	{
	////		// 後退
	////		tempEnemyPosition.Y -= m_moveSpeedY;
	////	}

	////	m_EnemyMovingDistance += m_moveSpeedY;

	////	// 修正後のアクター位置を設定
	////	SetActorLocation(tempEnemyPosition);

	////	// 可動範囲を超えたら
	////	if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
	////	{
	////		m_SwitchVector = true;
	////		m_StraightVector = !m_StraightVector;
	////	}

	////	m_EnemyState = ENEMY_STATE_MOVE;
	////	ChangeAnim();

	////	//---------------------------------------------------------------
	////	// 段差があったら乗り越えるみたいなプログラムがあってもいいかも
	////	//---------------------------------------------------------------
	////	break;

	////case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// ジャンプしてくる敵

	////	// 時間をカウント
	////	m_EnemyJumpDeltaTime += _deltaTime;

	////	// 待機時間を超えるまで動かない
	////	if (m_EnemyJumpDeltaTime < m_JumpWait)	return;

	////	//UE_LOG(LogTemp, Warning, TEXT("jumping"));

	////	// 鉛直投げ上げ
	////	tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
	////	tempEnemyPosition.Z += tempDeltaZ;

	////	// 初期位置より下に行った場合補正してリターン
	////	if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
	////	{
	////		// 初期位置の戻ったら時間の初期化
	////		SetActorLocation(tempEnemyPosition);
	////		m_EnemyJumpDeltaTime = 0.f;
	////		return;
	////	}

	////	// 値の反映
	////	SetActorLocation(tempEnemyPosition);

	////	m_EnemyState = ENEMY_STATE_MOVE;
	////	ChangeAnim();

	////	break;

	//case ENEMY_TYPE::ENEMY_TYPE_JUMP_STRAIGHT:
	//	//// 方向転換
	//	////if (m_SwitchVector)
	//	//{
	//	//	if (!m_StraightVector)
	//	//	{
	//	//		// 前向き
	//	//		tempEnemyRotation.Yaw = 90.f;
	//	//	}
	//	//	else
	//	//	{
	//	//		// 後ろ向き
	//	//		tempEnemyRotation.Yaw = 270.f;
	//	//	}

	//	//	//m_SwitchVector = false;
	//	//	SetActorRotation(tempEnemyRotation);
	//	//}

	//	//// 進む方向に応じて
	//	//if (!m_StraightVector)
	//	//{
	//	//	// 前進
	//	//	tempEnemyPosition.Y += m_moveSpeedY;
	//	//}
	//	//else
	//	//{
	//	//	// 後退
	//	//	tempEnemyPosition.Y -= m_moveSpeedY;
	//	//}

	//	//// 修正後のアクター位置を設定
	//	//SetActorLocation(tempEnemyPosition);

	//	//// 可動範囲を超えたら
	//	//if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
	//	//{
	//	//	//m_SwitchVector = true;
	//	//	m_StraightVector = !m_StraightVector;
	//	//}

	//	//// 時間をカウント
	//	//m_EnemyJumpDeltaTime += _deltaTime;

	//	//// 鉛直投げ上げ
	//	//tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
	//	//tempEnemyPosition.Z += tempDeltaZ;

	//	//// 初期位置より下に行った場合補正してリターン
	//	//if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
	//	//{
	//	//	// 初期位置の戻ったら時間の初期化
	//	//	SetActorLocation(tempEnemyPosition);
	//	//	m_EnemyJumpDeltaTime = 0.f;

	//	//	m_EnemyState = ENEMY_STATE_MOVE;
	//	//	ChangeAnim();
	//	//	return;
	//	//}

	//	//// 値の反映
	//	//SetActorLocation(tempEnemyPosition);
	//	//m_EnemyState = ENEMY_STATE_MOVE;
	//	ChangeAnim();

	//	//break;


	//case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// 弾発射敵

	//	if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
	//	{
	//		// 前向き
	//		tempEnemyRotation.Yaw = 90.f;
	//		m_StraightVector = false;
	//	}
	//	else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
	//	{
	//		// 後ろ向き
	//		tempEnemyRotation.Yaw = 270.f;
	//		m_StraightVector = true;
	//	}
	//	SetActorRotation(tempEnemyRotation);

	//	break;

	//case ENEMY_TYPE::ENEMY_TYPE_FRY:		// 空を飛ぶ敵
	//	//---------------------------------------------------------------
	//	// 今後によって書き換えます
	//	//---------------------------------------------------------------
	//	break;

	//default:
	//	break;
}

//// プレイヤーに当たった時の処理
//void AEnemyActor::EnemyStop()
//{
//	switch (m_EnemyType)
//	{
//	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵
//		// 方向転換
//		if (m_EnemyMovingDistance > m_ChangeVectorTolerance)
//		{
//			//m_SwitchVector = true;
//			m_StraightVector = !m_StraightVector;
//		}
//		break;
//
//	default:
//		break;
//	}
//}

// ダメージ関数
void AEnemyActor::EnemyDamage()
{
	// ダメージ状態だったら
	if ((m_EnemyState == ENEMY_STATE_DAMAGE)||(m_EnemyState == ENEMY_STATE_DESTROY)) return;

	m_EnemyState = ENEMY_STATE_DAMAGE;
	EnemyDamageEvent();

	if (m_EnemyHP > 0)
	{
		//------------------------------------------------
		//hitエフェクト生成が入る
		//------------------------------------------------
		// HPを減らす。ここはプレイヤーの攻撃値を参照するようにしてもいいかも
		--m_EnemyHP;
		m_EnemyState = ENEMY_STATE_DAMAGE;
		ChangeAnim();
	}

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
		Des();
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
