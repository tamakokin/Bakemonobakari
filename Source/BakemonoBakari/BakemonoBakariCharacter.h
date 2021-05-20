// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//更新履歴
//2021/04/23 更新者：陳　攻撃、しゃがみについて処理
//2021/05/07 更新者：陳　プレイヤーの基本情報（HPなど）
//2021/05/14 更新者：陳　飛翔距離が伸びる（ジャンプ）
//2021/05/17 更新者：陳　ダメージを受けたらノックバックする処理
//2021/05/19 更新者：陳　無敵時間フラグ
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "BakemonoBakariCharacter.generated.h"

UCLASS(config=Game)
class ABakemonoBakariCharacter : public ACharacter
{
	//プレイヤー情報格納用構造体 5/7
	struct Player_Info
	{
		float hp;

		Player_Info()
			:hp(100.0)
		{}
	};

	GENERATED_BODY()

	///** Side view camera */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UCameraComponent* SideViewCameraComponent;

	///** Camera boom positioning the camera beside the character */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpringArmComponent* CameraBoom;

protected:

	//スタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);

	///** Handle touch inputs. */
	//void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	///** Handle touch stop event. */
	//void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//ダメージを受ける処理 5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void TakeDamage(float _dmg);
	//ダメージを受けたらノックバックする 5/17
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void KnockBack(float _enemylocation);

	//攻撃しているかフラグ 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool IsAttack;
	//ジャンプしているかフラグ 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsJump;
	//しゃがんでいるかフラグ 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
		bool IsCrouch;
	//ダメージを受けているかフラグ 5/9
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDamage;
	//右方向に向いているかフラグ 5/10
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
		bool IsFaceRight;
	//飛翔距離伸びフラグ 5/14
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsHanging;
	//無敵時間フラグ 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invicible")
		bool IsInvincible;
	//オブジェクトと接触しているフラグ 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsOverlapping;
	//敵の水平位置 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float EnemyLocation;
	

	//Overlapテスト 5/14
	UPROPERTY(VisibleAnywhere, Category = "Overlap")
		class UCapsuleComponent* OverlapComponent;

public:
	ABakemonoBakariCharacter();

	//Tick関数
	virtual void Tick(float DeltaTime) override;

	//攻撃入力関数 4/23
	void Attack();

	//HPを取得　5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		int GetHP() { return m_info.hp; }

	//飛翔距離が伸びる関数 5/14
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void Hang();

	//Overlap関数 5/14
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Overlap関数 5/14
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	///** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	Player_Info m_info; //プレイヤー情報
};
