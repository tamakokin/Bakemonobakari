// ばけものチーム
// プレイヤーの方向に向けて回転する処理
#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_Rote_Component.generated.h"

class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UEnemy_Rote_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_Rote_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// プレイヤーの座標を取得
	UFUNCTION(BlueprintCallable)
	FVector GetPlayerPos();

	// 回転を行い初めて何カウントか
	UFUNCTION(BlueprintCallable)
		float GetRoteCount() const{ return m_RoteCount; }

	// 移動処理
	void EnemyRote();
	// プレイヤーの探索
	void SerchPlayer();
	void TurnRote(float _rote = 180.0f);

public:

	UPROPERTY(EditAnywhere)
		int  m_RoteCount;					// 回転をカウント

	UPROPERTY(EditAnywhere)
		float m_RoteSpeed;					// 方向転換の際の回転速度
private:
	float        m_RoteTarget;				// 回転の目標角度
	AEnemyActor* m_pEnemy;					// エネミーアクター
	AActor* m_pPlayerActor;					// プレイヤーアクター
};
