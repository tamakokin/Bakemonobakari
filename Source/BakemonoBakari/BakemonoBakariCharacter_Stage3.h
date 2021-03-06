// ばけものチーム
// 更新履歴
// 2021/07/28 更新者：陳　作成
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "BakemonoBakariCharacter_Stage3.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ABakemonoBakariCharacter_Stage3 : public ACharacter
{
	//プレイヤー情報格納用構造体 7/28
	struct Player_Info
	{
		float hp;

		Player_Info()
			:hp(100.0)
		{}
	};

	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABakemonoBakariCharacter_Stage3();

	//攻撃入力関数 4/23
	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);
	void MoveUp(float Val);

	//ダメージを受ける処理 7/29
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void TakeDamage(float _dmg);

	//攻撃しているかフラグ 7/28
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool IsAttack;
	//ジャンプしているかフラグ 7/28
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsJump;
	//しゃがんでいるかフラグ 7/29
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
		bool IsCrouch;
	//ダメージを受けているかフラグ 7/29
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDamage;
	//無敵時間フラグ 7/29
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invicible")
		bool IsInvincible;
	//HPが0になっているかフラグ 7/29
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDead;
	//敵と接触しているフラグ 7/29
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsEnemyContact;
	//右方向に向いているかフラグ 7/28
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
		bool IsFaceRight;

	//Overlapテスト 7/29
	UPROPERTY(VisibleAnywhere, Category = "Overlap")
		class UCapsuleComponent* OverlapComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Projectile class to spawn */
	//生成する撃つ弾のクラス 7/28
	UPROPERTY(EditAnywhere, Category = "Projectile")
		//TSubclassOf<class APlayerBullet> ProjectileClass;
		UClass * ProjectileClass;

	//HPを取得　7/29
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		int GetHP() { return m_info.hp; }

	//Overlap関数 7/29
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Overlap関数 5/29
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	Player_Info m_info; //プレイヤー情報

};
