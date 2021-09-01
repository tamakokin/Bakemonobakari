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

	void CheckLine();											// ライントレースを利用して前方あるアクターを判別して行動パターンを変える
public:

	UPROPERTY(EditAnywhere)
		float m_LineLength;										// ライントレースの長さ

	UPROPERTY(EditAnywhere)
		int m_AttackStartTiming;								// 攻撃開始タイミング
	UPROPERTY(EditAnywhere)
		int m_AttackEndTiming;									// 攻撃終了タイミング

public:
	UBoxCollision* m_boxCollision;								// 攻撃用の当たり判定

private:
	AEnemyActor* m_pEnemy;										// エネミーの基本処理
	UENEMY_TYPE_STRAIGHT_Component* m_pStraightComponent;		// 横移動をさせるコンポーネント
	UEnemy_Rote_Component* m_pEnemyRote;						// エネミーをプレイヤーの方向に向ける
	int m_AttackCount;										// 移動を開始するまでのカウント
	bool m_IsAttack;											// 攻撃中かどうか
	bool m_IsDepth;												// 奥行移動を行うかどうか
};
