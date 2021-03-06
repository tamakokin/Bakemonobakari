// ばけものチーム
// 河童の投げるきゅうりの移動処理

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
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
	// オーバーラップ関数
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// 消滅エフェクトを出す
	UFUNCTION(BlueprintImplementableEvent, Category = "Des")
		void DesEfect();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveStart(float _speed, bool _right);	// 移動開始

public:
	UPROPERTY(EditAnywhere)
		int m_LifeTime;							// 生存時間

	UPROPERTY(EditAnywhere)
		float m_Speed;							// 移動速度

	UPROPERTY(EditAnywhere)
		UCheckInScreen* m_pCheckInScreen;		// スクリーンに移るかどうかを調べる

	void Des();									// 非表示処理
private:
	bool m_IsMove;								// 移動を行うかどうか
	bool m_IsRight;								// 右に移動するかどうか
	int m_LifeCount;							// 生存時間をカウント

	UBoxComponent* m_pBox;
};
