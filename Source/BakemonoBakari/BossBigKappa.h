// ばけものチーム

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Spawn.h"
#include "EnemyActor.h"
#include "ENEMY_TOWER_Component.h"
#include "BossBigKappa.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ABossBigKappa : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossBigKappa();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// ボスのステータス
	enum BOSS_STATE
	{
		BOSS_STATE_NONE = 0,
		BOSS_STATE_IDLE,
		BOSS_STATE_ATTACK,
		BOSS_STATE_DAMAGE,
		BOSS_STATE_DESTROY,
	};

private:
	void Pattern1();						// フェーズ1の行動パターン
	void Pattern2();						// フェーズ2の行動パターン
	void Pattern3();						// フェーズ3の行動パターン
	void SpawnSetUp();						// アクターを生成するアクターを準備する
	void SerchPlayer();						// プレイヤーを探索する
	void SpawnKappa(int _num);				// 突撃するカッパを呼び出す
	void SpawnTowerKappa();					// タワーカッパを呼び出す
	void EnemyDamage();						// ダメージ処理
	void ChangeAnim();						// アニメーション変更処理
	void KillEnemy();						// 生成したエネミーを消す

public:
	// 死亡処理をBPから呼び出す
	UFUNCTION(BlueprintImplementableEvent, Category = "MyFunctions")
		void Des();


private:
	// オーバーラップ関数
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 尻子玉を呼び出す
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SpawnShirikodama();			

	// 状態の初期化
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReSetState();

	// アニメーション管理用
	// アイドル状態か
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsIdling() const { return m_IsIdling; }

	// ダメージを受けたか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDamage() const { return m_IsDamage; }

	// 死んだか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDestroy() const { return m_IsDestroy; }

	// 攻撃しているか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsAttacking() const { return m_IsAttacking; }

public:

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* m_pCapsuleComp;		//	ターゲット接触用コリジョン

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_pEnemyMesh;	// メッシュ

	UPROPERTY(EditAnywhere)
		TArray<int> m_AttackTiming;				// フェーズごとの攻撃を行なうタイミング

	UPROPERTY(EditAnywhere)
		TArray<int> m_KappaTiming;				// フェーズごとの出すカッパのタイミング

	UPROPERTY(EditAnywhere)
		TArray<int> m_KappaCount;				// フェーズごとの出すカッパの数

	UPROPERTY(EditAnywhere)
		FVector m_SpawnPos;						// カッパを出す座標

	UPROPERTY(EditAnywhere)
		float m_KnockbackPosY;					// フェーズごとにプレイヤーをこの座標まで移動させる

	UPROPERTY(EditAnywhere)
		float m_KnockbackSpeed;					// プレイヤーを移動させる速度

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_IsAction_bp;					// BP用行動可能フラグ

private:	

	int m_AttackCount;					// 攻撃までの時間をカウント
	int m_Phase;						// ボスカッパの攻撃パターン

	// アニメーション切り替え用
	bool	m_IsIdling;
	bool	m_IsDamage;
	bool	m_IsDestroy;
	bool	m_IsAttacking;

	bool    m_IsAction;					// 行動可能かどうか

	BOSS_STATE m_BossState;				// エネミーのステータス
	TArray<ASpawn*> m_Spawn;			// アクターを生成する
	AActor* m_pShirikodama;				// 尻子玉
	AActor* m_pPlayerActor;				// プレイヤーアクター
};
