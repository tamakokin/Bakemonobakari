// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//�X�V����
//2021/04/23 �X�V�ҁF�@�U���A���Ⴊ�݂ɂ��ď���
//2021/05/07 �X�V�ҁF�@�v���C���[�̊�{���iHP�Ȃǁj
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BakemonoBakariCharacter.generated.h"

UCLASS(config=Game)
class ABakemonoBakariCharacter : public ACharacter
{
	//�v���C���[���i�[�p�\���� 5/7
	struct Player_Info
	{
		int hp;

		Player_Info()
			:hp(100)
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

	//�X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
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

	//�_���[�W���󂯂鏈�� 5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
	void TakeDamage(int _dmg);

	//�U�����Ă��邩�t���O 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		bool IsAttack;
	//�W�����v���Ă��邩�t���O 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsJump;
	//���Ⴊ��ł��邩�t���O 4/23
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
		bool IsCrouch;
	//�_���[�W���󂯂Ă��邩�t���O 5/9
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDamage;

public:
	ABakemonoBakariCharacter();

	//Tick�֐�
	virtual void Tick(float DeltaTime) override;

	//�U�����͊֐� 4/23
	void Attack();

	//HP���擾�@5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		int GetHP() { return m_info.hp; }

	///** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	///** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	Player_Info m_info; //�v���C���[���
};
