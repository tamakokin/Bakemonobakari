// ばけものチーム
// 揺れる人玉
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_HITODAMA_Component.generated.h"

class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_HITODAMA_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_HITODAMA_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_Speed;			// 動く速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_Distance;		// 動く距離
private:
	FVector m_StartPos;			// 最初の座標
	AEnemyActor* m_pEnemy;		// エネミーの基本処理
	int m_MoveCount;			// 移動位置
	bool m_Re;					// 移動方向を反転
};
