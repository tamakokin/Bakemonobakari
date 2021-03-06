// ばけものチーム
// 作成者：大金巧侑 20201/05/27
// プレイヤーの座標取得しプレイヤーに近いスプライン上の座標を出す。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSpline.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ACameraSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraSpline();

protected:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 初期化（タイミングを合わせるためにCameraControlから呼ぶ）
	UFUNCTION(BlueprintCallable, Category = "Spline")
		void Start();

	// プレイヤーに近いスプライン座標を調べる
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
		void CheckPos();

	// 移動目標の座標を返す
	UFUNCTION(BlueprintCallable, Category = "Spline")
		FVector GetTargetPos()const
	{
		return m_TargetPos;
	}

	// プレイやーの座標を返す
	UFUNCTION(BlueprintCallable, Category = "Spline")
		FVector GetPlayerPos()const
	{
		return m_PlayerPos;
	}

private:
	void SetPos();

public:

	// スプライン上の座標
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		FVector m_TargetPos;	

	// プレイヤーの座標
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		FVector m_PlayerPos;

private:
	// プレイヤーのactor
	AActor* m_pPlayer;
};
