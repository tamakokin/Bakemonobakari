// ばけものチーム
// ボスカッパのタワーの行動パターン

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_TYPE_STRAIGHT_Component.h"
#include "ENEMY_TYPE_JUMP_Component.h"
#include "ENEMY_TOWER_Component.generated.h"

class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_TOWER_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_TOWER_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Move();

public:
	UPROPERTY(EditAnywhere)
		float m_LineLength;										// ライントレースの長さ

	UPROPERTY(EditAnywhere)
		float m_LimitPosY;										// タワーカッパの移動できる範囲

private:
	// ライントレース
	void CheckLine(FVector _startPos, FVector _endPos, FName _targetTag, TFunctionRef<void()> _function);	

	// プレイヤー近づいてきたらジャンプする
	void CheckPlayer();

	// 下に仲間のカッパがいる場合は一番下の個体じゃない
	void CheckEnemy();

private:
	bool m_IsBase;												// 一番下の個体かどうか

	UENEMY_TYPE_JUMP_Component* m_pJumpComponent;				// ジャンプ処理をせさるコンポーネント
	UENEMY_TYPE_STRAIGHT_Component* m_pStraightComponent;		// 横移動をさせるコンポーネント
	AEnemyActor* m_pEnemy;										// エネミーの基本処理
};
