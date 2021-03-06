// ばけものチーム
// 2021 05/14 大金巧侑
// カメラ内に入っているかをCameraControlに聞くコンポーネント

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CheckInScreen.generated.h"

class ACameraControl;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BAKEMONOBAKARI_API UCheckInScreen : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCheckInScreen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		// カメラに映るかを判定。敵から呼び出し
		bool Check(FVector _pos);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
		FVector m_Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	// このカメラに映る範囲か調べる
	ACameraControl* m_pCamera;
private:

};