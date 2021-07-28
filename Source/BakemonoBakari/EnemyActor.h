// AEnemyActor.h
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムを追加
//			：2021/5/17 エディタ側で敵の種類を文字列で変えられるように変更
//			：2021/5/23 消滅時の音を追加（伴野）
//			：2021/5/24 様々な敵形態を追加
//			：2021/5/29 画面外にいる場合は動かないようにする（大金）
//			：2021/6/ 9 アニメーションに必要な要素の追加
//			：2021/6/7  リスタート時にエネミーを初期化する
//			：			エネミーがやられた場合非表示にする

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyActor.generated.h"

class UCheckInScreen;

// 敵の種類
UENUM(BlueprintType)
enum class ENEMY_TYPE :uint8
{
	ENEMY_TYPE_NONE = 0,
	ENEMY_TYPE_STOP,			// 動かない
	ENEMY_TYPE_STRAIGHT,		// まっすぐ移動
	ENEMY_TYPE_JUMP,			// その場でジャンプ
	ENEMY_TYPE_JUMP_STRAIGHT,	// 移動しながらジャンプ
	ENEMY_TYPE_FIRING,			// 弾発射
	ENEMY_TYPE_FRY,				// 飛ぶ
};

UCLASS()
class BAKEMONOBAKARI_API AEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AEnemyActor();

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの処理
	virtual void Tick(float DeltaTime) override;

	// コライダー非表示処理
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void CollisionOff();

	// コライダー表示処理
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void CollisionOn();

	// メッシュの表示処理
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void MeshOn();

	// メッシュの非表示処理
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void MeshOff();

	//// ヒットエフェクトを出す処理
	//UFUNCTION(BlueprintCallable, Category = "Enemy")
	//	void Hit();

	// 初期化処理
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
		void ReStart();

	// 初期座標に移動
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStartPosition();

	// 状態の初期化
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReSetState();
private:
	// オーバーラップ関数
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	// ステータスコントロール
	void EnemyStatusControl(float _deltaTime);

	// アニメーション変更処理
	void ChangeAnim();

	// ダメージを受けた際に点滅させる
	void EnemyFlashing();

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Function")
		// ダメージ処理
		void EnemyDamage();

// アニメーション管理用
private:	
	// アイドル状態か
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsIdling() const { return m_bIdling; }

	// 動いているか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsMoving() const { return m_bMoving; }

	// ダメージを受けたか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDamage() const { return m_bDamage; }

	// 硬直したか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsStopping() const { return m_bStopping; }

	// 死んだか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDestroy() const { return m_bDestroy; }

	// 攻撃しているか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsAttacking() const { return m_bAttacking; }

public:
	// エネミーのステータス
	enum ENEMY_STATE
	{
		ENEMY_STATE_NONE = 0,
		ENEMY_STATE_MOVE,
		ENEMY_STATE_IDLE,
		ENEMY_STATE_ATTACK,
		ENEMY_STATE_DAMAGE,
		ENEMY_STATE_DESTROY,
		ENEMY_STATE_STOP,
	};

public:
	// エネミーのステータスをコンポーネントに伝える
	ENEMY_STATE GetEnmeyState()const { return m_EnemyState; }
	void SetEnemyState(const ENEMY_STATE _enemyState)
	{
		if ((m_EnemyState != ENEMY_STATE_DESTROY)&&(m_EnemyState != ENEMY_STATE_DAMAGE))
		{ 
			m_EnemyState = _enemyState; 
		} 
	}

	// プレイヤーがオーバーラップしたことをコンポーネントに伝える
	AActor* GetOverlappedActor()const { return m_pOverlappedActor; }
	void SetOverlappedActor(AActor* _actor) { m_pOverlappedActor = _actor; }

	// 初期座標をコンポーネントで扱う
	FVector GetStartPos()const { return m_initEnemyPosition; }

public:
	// blueprint EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* m_pCapsuleComp;		//	ターゲット接触用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* m_pEnemyMesh;	// メッシュ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* m_crashSound;			// 消滅時の音

private:
	UPROPERTY()
		USceneComponent* m_pBase;		// ベース
	UPROPERTY(EditAnywhere)
		FName m_tagName;				// タグ名

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 15))
		int m_EnemyHPMax;		// 敵の最大ＨＰ

	int m_EnemyHP;				// 敵の現在のHP

	UPROPERTY(EditAnywhere)
		int m_score;			// 敵を倒した場合に獲得できるスコア

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_DamageAnimationTime;		// ダメージのアニメーション時間

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackAnimationTime;		// 攻撃のアニメーション時間

		bool m_IsInScreen;					// 画面内いるかどうか

private:

	FVector m_initEnemyPosition;			// 初期位置
	FRotator m_StartRote;					// エネミーの初期回転

	// アニメーション切り替え用
	bool	m_bIdling;
	bool	m_bMoving;
	bool	m_bDamage;
	bool	m_bDestroy;
	bool	m_bStopping;
	bool	m_bAttacking;
	int	m_EnemyDamageCount;				// ダメージ後の無敵時間カウント用

private:
	ENEMY_STATE m_EnemyState;			// エネミーのステータス
	AActor* m_pOverlappedActor;			// オーバーラップしたアクター
	AActor* m_pPlayerCharacter;

	UCheckInScreen* m_pCheckInScreen;
};