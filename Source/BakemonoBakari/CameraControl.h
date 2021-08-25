// 2021 05/01 大金巧侑
// カメラの移動処理を管理する

// 76行目ににスクロールの止め方あり
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraControl.generated.h"

class ACameraSpline;

UCLASS()
class BAKEMONOBAKARI_API ACameraControl : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// パラメータを外部より調整（カメラのスクロールを止めたいときはカメラ停止位置のアクターを引数に入れてこれを呼ぶ）

	// カメラの移動をする際に追従するアクター、移動速度に設定する
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetActor(AActor* _targtActor, float _speed = 20.0f)
	{
		m_pActor = _targtActor;
		m_NowSpeed = _speed;
		m_Player = false;
	}

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
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		bool CheckInCamera(FVector _pos, FVector _size = FVector::ZeroVector);

private:
	// プレイヤーを検索する
	void Search();

	// スプラインを探索
	void SearchSpline();

	// カメラをプレイヤーに追従させる
	void MovePlayerCamera();

	// カメラを注目アクターに向けて移動させる(カメラの固定などに使用)
	void MoveCamera();

	// 静止時、移動時のカメラの拡縮を行う
	void ChangeScaleCamera(float _deltaTime);

	// 点Aと点Bの距離を計る
	float Measurement(FVector _a, FVector _b);

public:
	// カメラ移動に使用するスプラインの座標を取得する
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TArray<ACameraSpline*> m_pSpline;		// スプライン

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;				// プレイヤーを追従する際に調整に使う

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_TargetPos;					// 注目するアクターの座標
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;						// カメラの縦の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;						// カメラの横の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "拡大速度", ClampMin = "0", ClampMax = "10"))
		float m_ScaleUpTime;					// カメラの拡大速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "縮小速度", ClampMin = "0", ClampMax = "10"))
		float m_ScaleDownTime;					// カメラの縮小速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;						// カメラを配置する注目アクターからの奥行の距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "拡大率", ClampMin = "0", ClampMax = "200"))
		float m_Distance_ScaleUpMagnification;	// 移動時に拡大する倍率

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghWidth;						// 現在の座標とプレイヤーとの距離が長いなら移動させる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghHight;						// 現在の座標とプレイヤーとの距離が長いなら移動させる

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_MaxSpeed;						// カメラが移動できる範囲の最大値

private:

	AActor* m_pActor;							// プレイヤー以外のアクター

	// 移動目標にする座標
	float m_NowDistance;						// 現在カメラを配置する注目アクターからの奥行の距離
	float m_NowSpeed;							// カメラの現在の移動速度

	// プレイヤーの前方にカメラを移動させる
	FVector m_FrontPos;
	// プレイヤーの追従を行うかどうか
	bool m_Player;

	// 時間をカウント
	float m_CountTime;

	// プレイヤーアクタ
	AActor* m_pPlayerActor;

	// 移動するかどうか
	bool m_Move;
	bool m_PrevMove;

	// 右に移動するかどうか
	bool m_Right;
};