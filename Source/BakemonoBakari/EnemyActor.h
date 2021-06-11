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

private:
	// オーバーラップ関数
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	// ステータスコントロール
	void EnemyStatusControl(float _deltaTime);

	// 移動処理
	void EnemyMove(float _deltaTime);

	// 攻撃処理
	void EnemyAttack(float _deltaTime);

	// アニメーション変更処理
	void ChangeAnim();

	// プレイヤーに当たった時の処理
	void EnemyStop();

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

private:

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

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_moveSpeedY;		// 進行方向のスピード

	UPROPERTY(EditAnywhere)
		float m_moveRangeY;		// 進行方向の範囲

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 10.f))
		float m_InitVelocityZ;	// 上方向の初速

	UPROPERTY(EditAnywhere)
		float m_ChangeVectorTolerance;		// プレイヤーに当たった際の方向転換の許容移動範囲

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 29.4f))
		float m_JumpGravity;	// 重力加速度

	UPROPERTY(EditAnywhere)
		float m_altitudeLimit;	// 最大高度

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 15))
		int m_EnemyHP;		// 敵のＨＰ

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_JumpWait;		// ジャンプするまでの待機時間

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackDelay;		// 攻撃までの待機時間

	UPROPERTY(EditAnywhere)
		ENEMY_TYPE m_EnemyType;	// 敵の種類


	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_DamageAnimationTime;		// ダメージのアニメーション時間

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackAnimationTime;		// 攻撃のアニメーション時間




private:
	float	m_EnemyJumpDeltaTime;			// ジャンプしている時間
	float	m_EnemyMovingDistance;			// 向いている方向に移動した距離
	float	m_EnemyAttackingTime;			// 攻撃した後のディレイ
	bool	m_StraightVector;				// 向いている方向
	bool	m_SwitchVector;					// 右左のどちらを向いているかのフラグ

	FVector m_initEnemyPosition;			// 初期位置
	FVector m_prevEnemyPosition;			// 1フレーム前の自身の位置

	// アニメーション切り替え用
	bool	m_bIdling;
	bool	m_bMoving;
	bool	m_bDamage;
	bool	m_bDestroy;
	bool	m_bStopping;
	bool	m_bAttacking;

	float	m_EnemyDamageAnimationCount;	// ダメージアニメーションカウント用
	float	m_EnemyAttackAnimationCount;	// ダメージアニメーションカウント用



private:
	ENEMY_STATE m_EnemyState;			// エネミーのステータス
	AActor* m_pOverlappedActor;			// オーバーラップしたアクター
	AActor* m_pPlayerCharacter;

	UCheckInScreen* m_pCheckInScreen;
};