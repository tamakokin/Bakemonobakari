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

	// エネミーの移動処理
	void EnemyMove(float _deltaTime);
private:
	// ジャンプ
	FVector Jump(float _deltaTime);

	// 重力
	FVector Gravity(float _deltaTime);

	// ライントレースを利用して下のオブジェクトを確認
	bool CheckLine(FVector _MeshSize);
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

	UPROPERTY(EditAnywhere)
		bool    m_IsAir;						// 空中にいるかどうか

	UPROPERTY(EditAnywhere)
		bool    m_IsAnimation;					// アニメーションを変えるかどうか
private:
	AEnemyActor* m_pEnemy;						// エネミーの基本処理
	float   m_EnemyGravityDeltaTime;			// どれだけの間落下しているかを調べる
	bool	m_StraightVector;					// 向いている方向
	bool	m_SwitchVector;						// 右左のどちらを向いているかのフラグ
	bool    m_IsHit;							// 地面に当たっているかのフラグ
};
