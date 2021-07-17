// ばけものチーム
// 河童の投げるきゅうりの移動処理

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cucumber.generated.h"

class UCheckInScreen;

UCLASS()
class BAKEMONOBAKARI_API ACucumber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACucumber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveStart(float _speed, bool _right);		// 移動開始

public:
	UPROPERTY(EditAnywhere)
		float m_Speed;							// 移動速度
	UPROPERTY(EditAnywhere)
		UCheckInScreen* m_pCheckInScreen;		// スクリーンに移るかどうかを調べる
private:
	bool m_IsMove;								// 移動を行うかどうか
	bool m_IsRight;								// 右に移動するかどうか
};
