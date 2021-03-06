// ばけものチーム
// 2021/06/14 大金巧侑
// エネミーのリスポーン
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReStart.generated.h"

class AEnemyActor;

UCLASS()
class BAKEMONOBAKARI_API AReStart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReStart();

public:
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStartSet();

	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStartPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<AEnemyActor*> m_pEnemyActors;
	AEnemyActor* m_pEnemyActor;
};
