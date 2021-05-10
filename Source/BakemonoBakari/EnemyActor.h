// AEnemyActor.h
// 概要		：敵の行動プログラム(プロトタイプ
// 作成者	：上田悠晶
// 作成日	：2021/5/7
// 更新履歴	：2021/5/10 プログラムを追記


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyActor.generated.h"

UCLASS()
class POW_API AEnemyActor : public AActor
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

	// ダメージ処理
	void EnemyDamage();

	// 攻撃処理
	void EnemyAttack();


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

	// 敵の種類
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_NONE = 0,
		ENEMY_TYPE_STOP,
		ENEMY_TYPE_STRAIGHT,
		ENEMY_TYPE_JUMP,
		ENEMY_TYPE_FRY,
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
	UPROPERTY(EditAnywhere)
		float m_moveSpeedX;		// 進行方向のスピード

	UPROPERTY(EditAnywhere)
		float m_moveSpeedY;		// 上方向の移動量

	UPROPERTY(EditAnywhere)
		float m_altitudeLimit;	// 最大高度

	UPROPERTY(EditAnywhere)
		float m_EnemyHP;		// 敵のＨＰ

	UPROPERTY(EditAnywhere)
		float m_EnemyType_Any;	// 敵の種類入力用


private:
	ENEMY_STATE m_EnemyState;			// エネミーのステータス
	ENEMY_TYPE m_EnemyType;				// 敵の種類
	AActor* m_pOverlappedActor;			// オーバーラップしたアクター
};
