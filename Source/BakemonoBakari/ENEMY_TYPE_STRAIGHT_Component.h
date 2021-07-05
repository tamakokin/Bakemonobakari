// ばけものチーム

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_TYPE_STRAIGHT_Component.generated.h"

class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_TYPE_STRAIGHT_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_TYPE_STRAIGHT_Component();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SetIsMove(const bool _IsMove) { m_IsMove = _IsMove; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// エネミーの移動処理
	void EnemyMove(float _deltaTime);
private:
	// プレイヤーに当たった際の移動方向の反転
	void EnemyStop();

private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理

	FVector m_prevEnemyPosition;			// 1フレーム前の自身の位置
	bool	m_StraightVector;				// 向いている方向
	bool	m_SwitchVector;					// 右左のどちらを向いているかのフラグ

	float	m_EnemyMovingDistance;			// 向いている方向に移動した距離

public:
	UPROPERTY(EditAnywhere)
		bool m_IsMove;						// エネミーが移動するかどうか

	UPROPERTY(EditAnywhere)
		float m_ChangeVectorTolerance;		// プレイヤーに当たった際の方向転換の許容移動範囲

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 20.f))
		float m_moveSpeedY;					// 進行方向のスピード

	UPROPERTY(EditAnywhere)
	float	m_MaxMoveY;						// 移動できるY方向の最大座標

	UPROPERTY(EditAnywhere)
	float	m_MinMoveY;						// 移動できるY方向の最小座標
};
