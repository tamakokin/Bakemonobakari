// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//更新履歴
// 2021/04/23 更新者：陳　攻撃、しゃがみについて処理
// 2021/05/07 更新者：陳　プレイヤーの基本情報（HPなど）
// 2021/05/14 更新者：陳　飛翔距離が伸びる（ジャンプ）
// 2021/05/17 更新者：陳　ダメージを受けたらノックバックする処理
// 2021/05/19 更新者：陳　無敵時間フラグ
// 2021/05/26 更新者：陳　HPが0になった時の処理
// 2021/06/07 更新者：伴野　シーン開始時のフェードインの最中は入力を受け付けない
// 2021/06/07 更新者：大金　プレイヤー死亡時のリスポーン処理の追加
// 2021/07/05 更新者：伴野　梯子の上り下り機能の追加
// 2021/07/05 更新者：伴野　左右入力を受け取った際、一度変数に保存するよう変更
// 2021/07/08 更新者：伴野　梯子を登り切った際の処理を追加
// 2021/08/19 更新者：上田　移動攻撃時の慣性を追加
// 2021/08/20 更新者：山田　回復アイテムを取得したらhpを増やす処理
// 2021/08/27 更新者：松中　ジャンプフラグを外部から取得できるように更新
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "BakemonoBakariCharacter.generated.h"

class ACameraControl;

//近くにある梯子の向き 7/5伴野
UENUM(BlueprintType)
enum class ELadderDirection : uint8
{
	LD_None,
	LD_Right,
	LD_Left
};

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
	void MoveUp(float Val);

	// 左右上下入力の値を受け取って変数に保存する関数 7/5伴野
	void InputRight(float Val);
	void InputUp(float Val);

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

	//回復処理 8/20 山田
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void TakeRecovery(float _recovery);

	//ダメージを受けたらノックバックする 5/17
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void KnockBack(float _enemylocation);

	// 復帰処理
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStart();
public:
	// デバッグモードかのフラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool IsDebug;

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
	//梯子を掴んでいるかフラグ 7/5伴野
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		bool IsLadder;
	//梯子を登り切ったフラグ 7/8伴野
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		bool IsLadderClimb;
	//近くにある梯子の向き 7/5伴野
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		ELadderDirection LadderDir;
	//飛翔距離伸びフラグ 5/14
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsHanging;
	//無敵時間フラグ 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invicible")
		bool IsInvincible;
	//HPが0になっているかフラグ 5/26
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDead;
	//転落死亡フラグ 5/28
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsFallDead;
	//敵と接触しているフラグ 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsEnemyContack;
	//回復アイテムと接触しているフラグ 8/20
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsRecoveryContack;
	//敵の水平位置 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float EnemyLocation;

	// 左右上下入力の値を保存する変数 7/5伴野
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FVector2D InputValue;
	
	//フェードイン時の入力不可フラグ 6/7伴野
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool IsInputFadeIn;

	//Overlapテスト 5/14
	UPROPERTY(VisibleAnywhere, Category = "Overlap")
		class UCapsuleComponent* OverlapComponent;

	// カメラ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ACameraControl* m_pCamera;				

public:
	ABakemonoBakariCharacter();

	//Tick関数
	virtual void Tick(float DeltaTime) override;

	//攻撃入力関数 4/23
	void Attack();

	//HPを取得　5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		int GetHP() { return m_info.hp; }

	// 水平方向の入力値を保持 8/27
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		float GetHorizontal() { return m_Horizontal; }

	//飛翔距離が伸びる関数 5/14
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void Hang();

	// 梯子を掴むときの処理 7/5伴野
	void GraspLadder();

	// 梯子から手を放すときの処理 7/5伴野
	UFUNCTION(BlueprintCallable, Category = "Ladder")
		void LeaveLadder();

	// 梯子を上り切ったときの処理 7/5伴野
	void ClimbLadder(float DeltaTime);

	//フェードイン時の入力不可フラグを設定 6/7伴野
	UFUNCTION(BlueprintCallable, Category = "Input")
		void SetInputFadeIn(bool b) { IsInputFadeIn = b; }

	//Overlap関数 5/14
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Overlap関数 5/14
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// ジャンプしているかどうか(松中)
	bool GetIsJump() { return IsJump; }

private:
	void CheckLine();											// ライントレースを用いて足元に地面があるか調べる

private:
	Player_Info m_info;						//プレイヤー情報
	FVector m_ReStartPos;					// プレイヤーのリスポーン座標
	USkeletalMeshComponent* m_pMesh;		// メッシュ
	float m_Horizontal;						//水平方向の入力値保持
	bool m_IsGround;						// 接地しているかどうか
};