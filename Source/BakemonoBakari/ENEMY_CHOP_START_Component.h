// ばけものチーム
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_CHOP_START_Component.generated.h"
class UENEMY_TYPE_JUMP_Component;
class UENEMY_TYPE_STRAIGHT_Component;
class UEnemy_Rote_Component;
class ULINETRACE_Component;
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
		void SetJumpComponent(UENEMY_TYPE_JUMP_Component* _pJumpComponent) { m_pJumpComponent = _pJumpComponent; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SetStraightComponent(UENEMY_TYPE_STRAIGHT_Component* _pStraightComponent) { m_pStraightComponent = _pStraightComponent; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SetEnemyRoteComponent(UEnemy_Rote_Component* _pEnemyRote) { m_pEnemyRote = _pEnemyRote; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SetLineTrace(ULINETRACE_Component* _pLineTrace) { m_pLinetrace = _pLineTrace; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SetHitPlayer(bool _hitPlayer) { m_HitPlayer = _hitPlayer; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void SetHitGround(bool _hitGround) { m_HitGround = _hitGround; }
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void FinishAttack();
private:
	void EnemyMove(float _deltaTime);
public:
	UBoxCollision* m_boxCollision;								// 攻撃用の当たり判定
	UPROPERTY(EditAnywhere)
	int m_AttackStartTiming;									// 攻撃開始タイミング
	UPROPERTY(EditAnywhere)
	int m_AttackEndTiming;										// 攻撃終了タイミング

	UPROPERTY(EditAnywhere)
		bool m_HitPlayer;										// プレイヤーにあたったかどうか
	UPROPERTY(EditAnywhere)	
		bool m_HitGround;										// 地面ににあたったかどうか

private:
	AEnemyActor* m_pEnemy;										// エネミーの基本処理
	UENEMY_TYPE_JUMP_Component* m_pJumpComponent;				// ジャンプ処理をせさるコンポーネント
	UENEMY_TYPE_STRAIGHT_Component* m_pStraightComponent;		// 横移動をさせるコンポーネント
	UEnemy_Rote_Component* m_pEnemyRote;						// エネミーをプレイヤーの方向に向ける
	ULINETRACE_Component* m_pLinetrace;							// レイによる当たり判定を行う
	float m_AttackCount;										// 移動を開始するまでのカウント
	bool m_IsAttack;											// 攻撃中かどうか
};
