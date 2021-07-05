// ばけものチーム
// 2021/07/05 大金巧侑
// 一定時間ごとに横方向にジャンプする敵のコントロール
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_JUMP_START_Component.generated.h"

class UENEMY_TYPE_JUMP_Component;
class UENEMY_TYPE_STRAIGHT_Component;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_JUMP_START_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_JUMP_START_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		float m_JumpStartTiming;								// エネミーが移動するタイミング

private:
	UENEMY_TYPE_JUMP_Component* m_pJumpComponent;				// ジャンプ処理をせさるコンポーネント
	UENEMY_TYPE_STRAIGHT_Component* m_pStraightComponent;		// 横移動をさせるコンポーネント
	float m_JumpCount;											// 移動するまでのカウント
};
