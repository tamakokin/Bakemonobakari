// ばけものチーム
// ウェーブさせる敵

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_WAVE_Component.generated.h"

class AEnemyActor;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_WAVE_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_WAVE_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_Distance;		// 動く最大距離
private:
	FVector m_StartPos;			// 最初の座標

	AEnemyActor* m_pEnemy;		// エネミーの基本処理
};
