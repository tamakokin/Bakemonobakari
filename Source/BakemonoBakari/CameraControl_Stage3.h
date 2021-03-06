// ばけものチーム
// ステージ3のカメラの移動処理を管理する
// 更新履歴
// 2021/08/06 更新者：陳　作成

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraControl_Stage3.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ACameraControl_Stage3 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraControl_Stage3();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// プレイヤーの追従を行うかどうか
	bool m_Player;

	// プレイヤーアクタ
	AActor* m_pPlayerActor;

	// 移動目標にする座標
	float m_NowDistance;				// 現在カメラを配置する注目アクターからの奥行の距離
	float m_NowSpeed;					// カメラの現在の移動速度

	// プレイヤーの前方にカメラを移動させる
	FVector m_FrontPos;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;		// プレイヤーを追従する際に調整に使う
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_TargetPos;			// 注目するアクターの座標
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;				// カメラの縦の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;				// カメラの横の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_MaxSpeed;				// カメラが移動できる範囲の最大値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;				// カメラを配置する注目アクターからの奥行の距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_rotate;

public:
	// プレイヤーを追従するように設定する
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetPlayerActor()
	{
		m_NowSpeed = m_SpeedWidth;
		m_Player = true;
	}

	// カメラの距離を設定する
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetDistance(float _distance = 800.0f) { m_Distance = _distance; }

	// 引数で渡された座標がカメラに映るかどうかを調べる
	//UFUNCTION(BlueprintCallable, Category = "Camera Function")
		//bool CheckInCamera(FVector _pos, FVector _size = FVector::ZeroVector);

private:
	// プレイヤーを検索する
	void SearchPlayer();

	// カメラをプレイヤーに追従させる
	void MovePlayerCamera();

	// カメラを注目アクターに向けて移動させる(カメラの固定などに使用)
	//void MoveCamera();
};
