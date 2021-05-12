// ばけものチーム
// 作成： 松中海斗

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StopScroll.generated.h"

UCLASS()
class BAKEMONOBAKARI_API AStopScroll : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	AStopScroll();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
