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

	// ジャンプ開始
	void Jump(float _deltaTime);

	// 重力
	void Gravity(float _deltaTime);

	// 外部からジャンプ開始の指示をする
	void SetJump(bool _IsJump) { m_IsJump = _IsJump; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 敵が地面に触れた際空中状態をやめる（）
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 敵が地面から離れた場合に空中状態に
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void EndHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetIsAir() { return m_IsAir; }
private:
	// エネミーの移動処理
	void EnemyMove(float _deltaTime);
public:

	UPROPERTY(EditAnywhere)
	float m_JumpWait;						// ジャンプするまでの待機時間

	UPROPERTY(EditAnywhere)
	bool m_IsGravity;						// 重力をかけるかどうか

	UPROPERTY(EditAnywhere)
	bool m_IsJump;							// 上昇処理を行うかどうか

	UPROPERTY(EditAnywhere)
	float m_InitVelocityZ;					// ジャンプ力

	UPROPERTY(EditAnywhere)
	float m_JumpGravity;					// 重力

private:
	AEnemyActor* m_pEnemy;					// エネミーの基本処理

	float m_EnemyGravityDeltaTime;			// 
	bool	m_StraightVector;				// 向いている方向
	bool	m_SwitchVector;					// 右左のどちらを向いているかのフラグ
	bool    m_IsAir;						// 空中にいるかどうか
};
