// Shirikodama.h
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
//			：大金巧侑
// 作成日	：2021/07/09
// 更新履歴	：

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Shirikodama.generated.h"

UCLASS()
class BAKEMONOBAKARI_API AShirikodama : public AActor
{
	GENERATED_BODY()
	
public:	
	AShirikodama();

protected:
	virtual void BeginPlay() override;

	
public:	
	virtual void Tick(float DeltaTime) override;

	void MoveStart(float _speed, float _risingPower);

	// 敵が地面に触れた際空中状態をやめる（）
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 移動処理
	void Move();

public:
	UPROPERTY(EditAnywhere)
		float m_MoveSpeed;						// 移動速度
	UPROPERTY(EditAnywhere)
		float m_PlayerAttackMoveSpeed;			// プレイヤーが撃ち返した際の速度
	UPROPERTY(EditAnywhere)
		float m_RisingPower;					// 地面に当たった際に与える上昇力
	UPROPERTY(EditAnywhere)
		float m_LimitPosY;						// この座標を超えると消える

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* m_pMesh;		// メッシュ

public:
	// プレイヤーの攻撃状態かを判定
	bool GetPlayerAttack() { return m_IsPlayerAttack; }

	// 消す処理
	void Des();
private:

	float m_RisingPowerNow;						// 現在の上昇力
	float m_Gravity;							// 重力
	float m_Count;								// 空中でのカウント

	bool  m_IsSquashed;							// つぶれてい状態かどうか
	bool  m_IsPlayerAttack;						// プレイヤーの攻撃状態かどうか
};
