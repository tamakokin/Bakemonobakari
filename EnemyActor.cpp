// AEnemyActor.cpp
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムのコメントを追記
//			：2021/5/17 ジャンプする敵の行動プログラムを追加
//			：2021/5/24 ジャンプ移動する敵のプログラムを追加
//			：			移動する敵の反転バグを修正
//          ：			遠距離攻撃する敵の攻撃、行動プログラムを追加

#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"

// コンストラクタ・初期化
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pPlayerCharacter(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_EnemyJumpDeltaTime(0.f)
	, m_EnemyMovingDistance(0.f)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_prevEnemyPosition(FVector::ZeroVector)
	, m_StraightVector(false)
	, m_SwitchVector(false)
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

	// 初期位置の取得
	m_initEnemyPosition = GetActorLocation();

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

			if (pActor->ActorHasTag("PlayerPlayerCharacter"))
			{
				m_pPlayerCharacter = pActor;
				break;
			}
		}
	}
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

	// 移動関数
	EnemyMove(DeltaTime);
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
		else if (m_pOverlappedActor->ActorHasTag("PlayerCharacter"))
		{
			// 動きを止める
			EnemyStop();
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
	FRotator tempEnemyRotation = GetActorRotation();
	FVector tempPlayerCharacterPosition = m_pPlayerCharacter->GetActorLocation();

	// 変位Z
	float tempDeltaZ;

	switch (m_EnemyType)
	{

	case ENEMY_TYPE::ENEMY_TYPE_NONE:		// ダミー
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STOP:		// 動かない敵
		// 動かない
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵

		// 方向転換
		if (m_SwitchVector)
		{
			if (!m_StraightVector)
			{
				// 前向き
				tempEnemyRotation.Yaw = 90.f;
			}
			else
			{
				// 後ろ向き
				tempEnemyRotation.Yaw = 270.f;
			}

			m_SwitchVector = false;
			SetActorRotation(tempEnemyRotation);
		}

		m_prevEnemyPosition = tempEnemyPosition;

		// 進む方向に応じて
		if (!m_StraightVector)
		{
			// 前進
			tempEnemyPosition.Y += m_moveSpeedY;
		}
		else
		{
			// 後退
			tempEnemyPosition.Y -= m_moveSpeedY;
		}

		m_EnemyMovingDistance += m_moveSpeedY;

		// 修正後のアクター位置を設定
		SetActorLocation(tempEnemyPosition);

		// 可動範囲を超えたら
		if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
		{
			m_SwitchVector = true;
			m_StraightVector = !m_StraightVector;
		}

		//---------------------------------------------------------------
		// 段差があったら乗り越えるみたいなプログラムがあってもいいかも
		//---------------------------------------------------------------
		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// ジャンプしてくる敵

		// 時間をカウント
		m_EnemyJumpDeltaTime += _deltaTime;

		// 待機時間を超えるまで動かない
		if (m_EnemyJumpDeltaTime < m_JumpWait)	return;

		UE_LOG(LogTemp, Warning, TEXT("jumping"));

		// 鉛直投げ上げ
		tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
		tempEnemyPosition.Z += tempDeltaZ;

		// 初期位置より下に行った場合補正してリターン
		if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
		{
			// 初期位置の戻ったら時間の初期化
			SetActorLocation(tempEnemyPosition);
			m_EnemyJumpDeltaTime = 0.f;
			return;
		}

		// 値の反映
		SetActorLocation(tempEnemyPosition);

		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP_STRAIGHT:
		// 方向転換
		if (m_SwitchVector)
		{
			if (!m_StraightVector)
			{
				// 前向き
				tempEnemyRotation.Yaw = 90.f;
			}
			else
			{
				// 後ろ向き
				tempEnemyRotation.Yaw = 270.f;
			}

			m_SwitchVector = false;
			SetActorRotation(tempEnemyRotation);
		}

		// 進む方向に応じて
		if (!m_StraightVector)
		{
			// 前進
			tempEnemyPosition.Y += m_moveSpeedY;
		}
		else
		{
			// 後退
			tempEnemyPosition.Y -= m_moveSpeedY;
		}

		// 修正後のアクター位置を設定
		SetActorLocation(tempEnemyPosition);

		// 可動範囲を超えたら
		if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
		{
			m_SwitchVector = true;
			m_StraightVector = !m_StraightVector;
		}

		// 時間をカウント
		m_EnemyJumpDeltaTime += _deltaTime;

		// 鉛直投げ上げ
		tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
		tempEnemyPosition.Z += tempDeltaZ;

		// 初期位置より下に行った場合補正してリターン
		if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
		{
			// 初期位置の戻ったら時間の初期化
			SetActorLocation(tempEnemyPosition);
			m_EnemyJumpDeltaTime = 0.f;
			return;
		}

		// 値の反映
		SetActorLocation(tempEnemyPosition);

		break;
	case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// 弾発射敵

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

		break;

	case ENEMY_TYPE::ENEMY_TYPE_FRY:		// 空を飛ぶ敵
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

	// アクター生成用
	FString BulletPath;
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor;

	//---------------------------------------------------------------
	// 攻撃方法が不明のため条件分岐のみ記載します
	//---------------------------------------------------------------
	switch (m_EnemyType)
	{

	case ENEMY_TYPE::ENEMY_TYPE_NONE:		// ダミー
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STOP:		// 動かない敵
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵
		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// ジャンプしてくる敵
		break;

	case ENEMY_TYPE::ENEMY_TYPE_FRY:		// 空を飛ぶ敵
		break;

	case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// 弾発射

		// BPがあるパスを指定
		BulletPath = "/Game/Blueprints/EnemyBulletBP.EnemyBulletBP_c";

		// パス指定したBPのアクタークラスを格納
		subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

		// 位置と回転を設定
		location = GetActorLocation(); //+ m_pCapsuleComp->GetRightVector() * 10;
		rotation = GetActorRotation();

		// rotation.Yaw -= 90.f;
		// 設定した値を反映してスポーン
		actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);
		break;

	default:
		break;
	}
}

// プレイヤーに当たった時の処理
void AEnemyActor::EnemyStop()
{
	switch (m_EnemyType)
	{
	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// 真っすぐ突っ込んでくる敵
		// 方向転換
		if (m_EnemyMovingDistance > m_ChangeVectorTolerance)
		{
			m_SwitchVector = true;
			m_StraightVector = !m_StraightVector;
		}
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