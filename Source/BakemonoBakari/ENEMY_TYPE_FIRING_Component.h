// ばけものチーム

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_TYPE_FIRING_Component.generated.h"

class AEnemyActor;
class ACucumber;
class UEnemy_Rote_Component;
class ASpawn;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BAKEMONOBAKARI_API UENEMY_TYPE_FIRING_Component : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UENEMY_TYPE_FIRING_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	// キュウリを出す
	UFUNCTION(BlueprintCallable)
		void Spawn();
	// キュウリを投げる
	UFUNCTION(BlueprintCallable)
		void EnemyAttack();
	// 投げ終える
	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	// きゅうりを消す
	UFUNCTION(BlueprintCallable)
		void DestroyCucumber();

public:
	UPROPERTY(EditAnywhere)
		float m_AttackDelay;				// 攻撃するまでの時間
	UPROPERTY(EditAnywhere)
		float m_Speed;						// 投げる物の速度
	UPROPERTY(EditAnywhere)
		ACucumber* m_pCucumber;				// 投げる物
	UPROPERTY(EditAnywhere)
		FString m_Path;						// 投げるアクターのパス

private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理
	UEnemy_Rote_Component* m_pEnemyRote;	// エネミーをプレイヤーの方向に回転させる
	float m_EnemyAttackingCount;			// 攻撃するまでのカウント

	ASpawn* m_Spawn;						// 投げる用のアクターを生成する
};
