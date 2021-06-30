// ばけものチーム

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_TYPE_FIRING_Component.generated.h"

class AEnemyActor;

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
	// 移動処理
	void EnemyMove(float _deltaTime);

	// 攻撃処理
	UFUNCTION()
	void EnemyAttack();

	// プレイヤーの探索
	void SerchPlayer();

public:
	UPROPERTY(EditAnywhere)
	float m_AttackDelay;					// 攻撃するまでの時間
private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理
	AActor* m_pPlayerActor;					// プレイヤーアクター

	bool m_StraightVector;					// 向いている方向

	float m_EnemyAttackingCount;			// 攻撃するまでのカウント
};
