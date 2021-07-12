// Shirikodama.h
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shirikodama.generated.h"

UCLASS()
class BAKEMONOBAKARI_API AShirikodama : public AActor
{
	GENERATED_BODY()
	
public:	
	AShirikodama();

private:
	

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", DisplayName = "速度")
		float v;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", DisplayName = "波長")
		float waveLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", DisplayName = "振幅")
		float amplitude;
	float time;
};
