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
	// エネミーの移動処理
	void EnemyMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理
public:

	UPROPERTY(EditAnywhere)
		bool    m_IsTurn;					// 連続で回転しないようにフラグで管理

	UPROPERTY(EditAnywhere)
		bool m_IsMove;						// エネミーが移動するかどうか

	UPROPERTY(EditAnywhere)
		bool m_IsAnimation;					// エネミーのアニメーションを変更するかどうか

	UPROPERTY(EditAnywhere)
		int  m_TurnTiming;					// 回転を行えるようになるタイミング

	UPROPERTY(EditAnywhere)
		int  m_RoteCount;					// 回転をカウント

	UPROPERTY(EditAnywhere)
		int  m_TurnCount;					// 前回回転してからどれだけ経ったかカウント

	UPROPERTY(EditAnywhere)
		float m_ChangeVectorTolerance;		// プレイヤーに当たった際の方向転換の許容移動範囲

	UPROPERTY(EditAnywhere)
		float m_RoteSpeed;					// 方向転換の際の回転速度

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 20.f))
		float m_moveSpeedY;					// 進行方向のスピード

	UPROPERTY(EditAnywhere)
		float	m_MaxMoveY;					// 移動できるY方向の最大座標

	UPROPERTY(EditAnywhere)
		float	m_MinMoveY;					// 移動できるY方向の最小座標
};
