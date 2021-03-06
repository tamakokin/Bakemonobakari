// ばけものチーム
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_CHOP_START_Component.generated.h"
class UENEMY_TYPE_JUMP_Component;
class UENEMY_TYPE_STRAIGHT_Component;
class UEnemy_Rote_Component;
class UBoxCollision;
class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_CHOP_START_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_CHOP_START_Component();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void FinishAttack();
private:
	void EnemyMove(float _deltaTime);

	void CheckLineAttackStart();								// イントレースを利用して前方あるアクターを判別してプレイヤーなら攻撃壁なら方向転換をする
	void CheckLineMoveStart();									// ライントレースを利用して前方あるアクターを判別してプレイヤーなら移動を開始する
public:

	UPROPERTY(EditAnywhere)
		float m_AttackStartLineLength;							// ライントレースの長さ

	UPROPERTY(EditAnywhere)
		float m_MoveStartLineLength;							// ライントレースの長さ

	UPROPERTY(EditAnywhere)
		int m_RandomTurnprobability;							// どのくらいの確率で振り向くか

public:
	UBoxCollision* m_boxCollision;								// 攻撃用の当たり判定

private:
	AEnemyActor* m_pEnemy;										// エネミーの基本処理
	UENEMY_TYPE_STRAIGHT_Component* m_pStraightComponent;		// 横移動をさせるコンポーネント
	UEnemy_Rote_Component* m_pEnemyRote;						// エネミーをプレイヤーの方向に向ける
	bool m_IsLookPlayer;										// プレイヤーを観ているかどうか
	bool m_IsAttack;											// 攻撃中かどうか
	bool m_IsDepth;												// 奥行移動を行うかどうか
};
