// ばけものチーム
// 更新履歴
// 2021/08/27 更新者：山田　作成

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Loading.generated.h"

#define CHECKPOINT 4 // チェックポイント数

UCLASS()
class BAKEMONOBAKARI_API ALoading : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALoading();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// ブループリントで扱う関数
	UFUNCTION(BlueprintCallable, Category = "MyAnimeFunction")
		int GetWalkAnim()const { return m_EnemyAnim[0]; }
	UFUNCTION(BlueprintCallable, Category = "MyAnimeFunction")
		int GetDamageAnim()const { return m_EnemyAnim[1]; }

protected:
	// 毎フレーム更新処理
	void UpdateMove(float _deltaTime);

	// アニメーション初期化
	void Reset(bool _anim);
	// 方向を取得
	void UpdateDirection(float _deltaTime);
	// アクションボタン
	void ActionBottan();
	// 計算
	float Calculation(float _Now);

protected:
	// チェックポイントの座標保存用配列
	UPROPERTY(EditAnywhere, Category = "Location")
		FVector m_EnemyLocation[CHECKPOINT];
	// 回転配列
	UPROPERTY(EditAnywhere, Category = "Location")
		float m_EnemyRotation[CHECKPOINT];
	// アングル
	UPROPERTY(EditAnywhere, Category = "Location")
		float m_angle;
	// アニメーション配列
	UPROPERTY(EditAnywhere, Category = "Animation")
		int m_EnemyAnim[CHECKPOINT];
	// 時間カウント
	UPROPERTY(EditAnywhere, Category = "Location")
		float m_TimeCnt;
	// チェックカウント
	UPROPERTY(VisibleAnywhere, Category = "Location")
		int m_CheckCnt;
	// 回転ができたのかチェック
	UPROPERTY(VisibleAnywhere, Category = "Check")
		bool m_Completion;
	UPROPERTY(VisibleAnywhere, Category = "Check")
		bool m_Reset;
	// スケルトン
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		USkeletalMeshComponent* m_pMesh;

private:
	bool m_CheckBottan;
	float m_Fps;
};
