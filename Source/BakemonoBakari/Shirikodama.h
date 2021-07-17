// Shirikodama.h
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
//			：大金巧侑
// 作成日	：2021/07/09
// 更新履歴	：

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shirikodama.generated.h"

class UCheckInScreen;

UCLASS()
class BAKEMONOBAKARI_API AShirikodama : public AActor
{
	GENERATED_BODY()
	
public:	
	AShirikodama();

protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

	void MoveStart(float _speed, float _risingPower);

	// 敵が地面に触れた際空中状態をやめる（）
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	UPROPERTY(EditAnywhere)
		float m_MoveSpeed;						// 移動速度
	UPROPERTY(EditAnywhere)
		float m_RisingPower;					// 地面に当たった際に与える上昇力
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_pMesh;		// メッシュ

	UPROPERTY(EditAnywhere)
		UCheckInScreen* m_pCheckInScreen;		// スクリーンに移るかどうかを調べる

private:

	int m_SquashedCount;						// つぶれ具合のカウント

	float m_RisingPowerNow;						// 現在の上昇力
	float m_Gravity;							// 重力
	float m_Count;								// 空中でのカウント

	bool  m_IsSquashed;							// つぶれてい状態かどうか
};
