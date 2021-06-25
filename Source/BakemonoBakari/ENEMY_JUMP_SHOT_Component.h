// ばけものチーム
// ジャンプして、一旦空中で停止し球を撃つエネミー

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_JUMP_SHOT_Component.generated.h"

class AEnemyActor;
class UENEMY_TYPE_JUMP_Component;
class UENEMY_TYPE_FIRING_Component;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_JUMP_SHOT_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_JUMP_SHOT_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// 敵が地面に触れた際空中状態をやめる（）
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere)
		float m_ShotHeight;		// この高さになったときに弾を出す。

private:

	bool m_Air;					// 空中かどうか
	float m_OldHeight;			// ジャンプ前の高さ
	AEnemyActor* m_pEnemy;		// エネミーの基本処理

	UENEMY_TYPE_JUMP_Component   * m_pJump;
	UENEMY_TYPE_FIRING_Component * m_pFiring;
};
