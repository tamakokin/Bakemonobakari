// TowerKappa.h
// 概要		：タワーかっぱの挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：2021/07/15 更新者：陳　タワーを作る関数と必要の情報変数を作る
//　　　　　：2021/07/19 更新者：陳　タワーを作る関数と必要の情報変数を更新

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "Engine.h"
#include "TowerKappa.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ATowerKappa : public AEnemyActor
{
	GENERATED_BODY()
public:
	ATowerKappa();

private:
	void AttackPhase1();
	void AttackPhase2();
	void AttackPhase3();

protected:
	virtual void BeginPlay() override;

public:
	// タワーを作る関数　(陳7/15)
	UFUNCTION(BlueprintCallable, Category = "Wall")
		void BuildTower(int _phase);

	// タワーが攻撃を受ける関数　(陳7/15)
	UFUNCTION(BlueprintCallable, Category = "Wall")
		void BreakTower();

	UFUNCTION(BlueprintImplementableEvent, Category = "Wall")
		// タワー作るイベント　(陳7/19)
		void CreateEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
		// ダメージイベント　(陳7/19)
		void DamageEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Destroy")
		// 削除イベント　(陳7/19)
		void DestroyEvent();

public:
	virtual void Tick(float DeltaTime) override;

	// タワーになる番号	(陳7/15)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		int m_Number;
	// タワーの個数	(陳7/15)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		int _TowerHP;
	// ボスのフェース状態	(陳7/19)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		int _BossPhase;

	// タワーになっているかフラグ	(陳7/15)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		bool _IsTower;
	// ダメージを受けているかフラグ	(陳7/19)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		bool _IsDamage;
	// 無敵のフラグ	(陳7/19)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wall")
		bool _IsInvincible;

};
