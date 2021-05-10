// 2021 05/01 大金巧侑
// カメラの移動処理を管理する

// 76行目ににスクロールの止め方あり
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraControl.generated.h"

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

private:
	
	AActor* m_pActor;					// プレイヤー以外のアクター

	// 移動目標にする座標
	float m_NowDistance;				// 現在カメラを配置する注目アクターからの奥行の距離
	float m_NowSpeed;					// カメラの現在の移動速度

	FVector m_TargetPos;

	// 前回の目標座標
	FVector m_OldPos;

	// プレイヤーの追従を行うかどうか
	bool m_Player;

	// カメラの揺れに使用する
	int m_shockCount;				// 揺れる回数	
	int m_shockTiming;				// 揺れるタイミング
	float m_shockMaxWidth;			// 横の揺れ幅
	float m_shockMaxHeight;			// 縦の揺れ幅
	bool m_shockStart;				// カメラを揺らすかどうか

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	// カメラが注目するアクターを取得
	AActor* m_pPlayerActor;				// プレイヤー

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;		// プレイヤーを追従する際に調整に使う

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;					// カメラの縦の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;					// カメラの横の移動速度

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;				// カメラを配置する注目アクターからの奥行の距離

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Lengh;					// 現在の座標と目標座標との距離が短いなら移動させない
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MaxPos;				// カメラが移動できる範囲の最大値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MinPos;				// カメラが移動できる範囲の最小値
private:
	// プレイヤーを検索する
	void SearchPlayer();

public:	
	// パラメータを外部より調整（カメラのスクロールを止めたいときはカメラ停止位置のアクターを引数に入れてこれを呼ぶ）
	UFUNCTION(BlueprintCallable, Category = "Ball Function")
	void SetTargetActor(AActor* _targtActor,float _speed)						// 注目するアクター、移動速度の変更
	{ 
		m_pActor = _targtActor;
		m_NowSpeed = _speed;
		m_Player = false;
	};

	void SetTargetPlayerActor()													// プレイヤーを注目アクターにする
	{
		m_pActor = m_pPlayerActor;
		m_NowSpeed = m_SpeedWidth;
		m_Player = true;
	};

	// 揺れに使用する
	UFUNCTION(BlueprintCallable, Category = "Ball Function")
	void SetShock(int _shockCount = 300,int _shockTiming = 10, float _shockMaxX = 200, float _shockMaxY = 200)
	{
		m_shockCount = _shockCount;
		m_shockTiming = _shockTiming;
		m_shockMaxWidth = _shockMaxX;
		m_shockMaxHeight = _shockMaxY;
		m_shockStart = true;
	}

	void SetDistance(float const _distance) { m_Distance = _distance; }			// カメラの距離を設定する

	// 引数で渡された座標がカメラに映るかどうかを調べる
	UFUNCTION(BlueprintCallable, Category = "Ball Function")
	bool CheckInCamera(FVector _pos);

	private:
		// カメラが範囲外にいる場合範囲に戻す
		void CheckInPos();

		// プレイヤーの向きによって中心座標を変更
		void NoticePlayer();

		// カメラをプレイヤーに追従させる
		void MovePlayerCamera();

		// カメラを注目アクターに向けて移動させる移動させる
		void MoveCamera();

		// カメラを揺らす
		void Shock();

		// 点Aと点Bの距離を計る
		float Measurement(FVector _a, FVector _b);
};
