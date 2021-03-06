// ばけものチーム
// 更新履歴
// 2021/07/05 作成者：伴野

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 梯子の長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder", meta = (ClampMin = 300))
		float ladderLength;

};
