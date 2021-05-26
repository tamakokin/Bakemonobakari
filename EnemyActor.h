// AEnemyActor.h
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムを追加
//			：2021/5/17 エディタ側で敵の種類を文字列で変えられるように変更


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyActor.generated.h"

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
	// 移動処理
	void EnemyMove(float _deltaTime);

	// 攻撃処理
	void EnemyAttack();

	// プレイヤーに当たった時の処理
	void EnemyStop();

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Function")
		// ダメージ処理
		void EnemyDamage();

private:

	// エネミーのステータス
	enum ENEMY_STATE
	{
		ENEMY_STATE_NONE = 0,
		ENEMY_STATE_MOVE,
		ENEMY_STATE_IDLE,
		ENEMY_STATE_DAMAGE,
		ENEMY_STATE_STOP,
	};

public:
	// blueprint EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* m_pCapsuleComp;		//	ターゲット接触用コリジョン
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* m_pEnemyMesh;	// メッシュ

private:
	UPROPERTY()
		USceneComponent* m_pBase;		// ベース
	UPROPERTY(EditAnywhere)
		FName m_tagName;				// タグ名

private:
	UPROPERTY(EditAnywhere ,meta = (ClampMin = 0.f,ClampMax = 5.f))
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

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 3))
		int m_EnemyHP;		// 敵のＨＰ

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_JumpWait;		// ジャンプするまでの待機時間

	UPROPERTY(EditAnywhere)
		ENEMY_TYPE m_EnemyType;	// 敵の種類



private:
	float	m_EnemyJumpDeltaTime;			// ジャンプしている時間
	float	m_EnemyMovingDistance;			// 向いている方向に移動した距離
	bool	m_StraightVector;				// 向いている方向
	bool	m_SwitchVector;					// 右左のどちらを向いているかのフラグ

	FVector m_initEnemyPosition;			// 初期位置
	FVector m_prevEnemyPosition;			// 1フレーム前の自身の位置

private:
	ENEMY_STATE m_EnemyState;			// エネミーのステータス
	AActor* m_pOverlappedActor;			// オーバーラップしたアクター
	AActor* m_pPlayerCharacter;
};