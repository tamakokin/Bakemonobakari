// Stage01_SmallBoss.h
// 概要		：ステージ１の小さいボスの挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "Stage01_SmallBoss.generated.h"

UCLASS()
class BAKEMONOBAKARI_API AStage01_SmallBoss : public AEnemyActor
{
	GENERATED_BODY()
public:
	AStage01_SmallBoss();

private:
	// Y方向の速度の計算
	void MovementY(float _DeltaTime);

public:
	// 毎フレームの処理
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmallBoss|Movement", DisplayName = "初速度")
		float m_v0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "SmallBoss|Movement", DisplayName = "加速度")
		float m_a;
	float m_time;
};
