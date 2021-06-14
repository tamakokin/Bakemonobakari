// ばけものチーム

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ENEMY_TYPE_JUMP_Component.generated.h"

class AEnemyActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BAKEMONOBAKARI_API UENEMY_TYPE_JUMP_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UENEMY_TYPE_JUMP_Component();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void EnemyMove(float _deltaTime);
		
public:
	UPROPERTY(EditAnywhere)
		float m_ChangeVectorTolerance;		// プレイヤーに当たった際の方向転換の許容移動範囲

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_moveSpeedY;					// 進行方向のスピード

	UPROPERTY(EditAnywhere)
		float	m_MaxMoveY;					// 移動できるY方向の最大座標

	UPROPERTY(EditAnywhere)
		float	m_MinMoveY;					// 移動できるY方向の最小座標

	UPROPERTY(EditAnywhere)
	float m_JumpWait;						// ジャンプするまでの待機時間

	UPROPERTY(EditAnywhere)
	float m_InitVelocityZ;					// ジャンプ力

	UPROPERTY(EditAnywhere)
	float m_JumpGravity;					// 重力

private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理

	float m_EnemyJumpDeltaTime;				// 時間をカウントする

	bool	m_StraightVector;				// 向いている方向
	bool	m_SwitchVector;					// 右左のどちらを向いているかのフラグ
};
