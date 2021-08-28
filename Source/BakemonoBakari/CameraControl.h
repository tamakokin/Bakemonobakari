// 2021 05/01 大金巧侑 カメラの移動処理を管理する
// 2021/08/25 松中海斗 静止時にカメラを引く、移動時にカメラを近づけるように更新
// 2021/08/27 松中海斗 プレイヤーキャラクターの型を変更、スプラインに沿って移動するかどうかのフラグを追加
// 2021/08/28 コントローラーからの入力でカメラを移動できるようにする

// 76行目ににスクロールの止め方あり
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BakemonoBakariCharacter.h"
#include "CameraControl.generated.h"

class ACameraSpline;
class ABakemonoBakariCharacter;
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

	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetMoveCameraY(float _move) { m_MoveCamera.Y += _move; }

	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetMoveCameraZ(float _move) { m_MoveCamera.Z += _move; }

	// スプラインを探索
	void SearchSpline();

private:
	// プレイヤーを検索する
	void Search();

	// カメラをプレイヤーに追従させる
	void MovePlayerCamera(float _deltaTime);

	// カメラを注目アクターに向けて移動させる(カメラの固定などに使用)
	void MoveCamera();

	// 点Aと点Bの距離を計る
	float Measurement(FVector _a, FVector _b);

public:
	// カメラ移動に使用するスプラインの座標を取得する
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TArray<ACameraSpline*> m_pSpline;		// スプライン

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Position")
		FVector m_AdjustmentPos;				// プレイヤーを追従する際に調整に使う

	UPROPERTY(VisibleAnywhere, Category = "Camera|Position")
		FVector m_TargetPos;					// 注目するアクターの座標

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Position")
		FVector m_CameraMoveReturn;				// カメラを移動する際の限界値
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_SpeedHight;						// カメラの縦の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_SpeedWidth;						// カメラの横の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "10"))
		float m_ScaleUpTime;					// カメラの拡大速度（松中）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "10"))
		float m_ScaleDownTime;					// カメラの縮小速度（松中）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;						// カメラを配置する注目アクターからの奥行の距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "200"))
		float m_Distance_ScaleUpMagnification;	// 移動時に拡大する倍率（松中）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Length")
		float m_LenghWidth;						// 現在の座標とプレイヤーとの距離が長いなら移動させる
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Length")
		float m_LenghHight;						// 現在の座標とプレイヤーとの距離が長いなら移動させる

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_MaxSpeed;						// カメラが移動できる範囲の最大値

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float  m_SwayingMax;					// プレイヤーが地面に当たった時に揺らす初期値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float  m_SwayingAdd;					// プレイヤーが地面に当たった時に揺らす現在の揺れの増幅量

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayPriority = "1"))
		bool m_bSplineMode;						// スプラインに沿って移動するかどうか（松中）

private:

	AActor* m_pActor;							// プレイヤー以外のアクター

	// 移動目標にする座標
	float m_NowDistance;						// 現在カメラを配置する注目アクターからの奥行の距離
	float m_NowSpeed;							// カメラの現在の移動速度

	// プレイヤーが地面に当たった時に揺らす現在の揺れの総量
	float  m_SwayingNow;
	// プレイヤーが地面に当たった時に揺れを起こすかどうか
	bool m_IsSway;

	// プレイヤーの前方にカメラを移動させる
	FVector m_FrontPos;

	// 入力によってカメラを移動させる
	FVector m_MoveCamera;

	ABakemonoBakariCharacter* m_pPlayerActor;

	// プレイヤーの追従を行うかどうか
	bool m_Player;

	// 時間をカウント
	float m_CountTime;
	bool m_bCount;
	bool m_Move;
	bool m_PrevMove;

	// 右に移動するかどうか
	bool m_Right;

	// 切り替える前のカメラの座標
	FVector m_PrevChangeCameraPos;
	// ひとつ前のカメラの座標
	FVector m_PrevCameraPos;
	// 一つ前のジャンプフラグ
	bool m_PrevIsJump;
};