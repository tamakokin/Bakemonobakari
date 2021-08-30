// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//�X�V����
// 2021/04/23 �X�V�ҁF�@�U���A���Ⴊ�݂ɂ��ď���
// 2021/05/07 �X�V�ҁF�@�v���C���[�̊�{���iHP�Ȃǁj
// 2021/05/14 �X�V�ҁF�@���ċ������L�т�i�W�����v�j
// 2021/05/17 �X�V�ҁF�@�_���[�W���󂯂���m�b�N�o�b�N���鏈��
// 2021/05/19 �X�V�ҁF�@���G���ԃt���O
// 2021/05/26 �X�V�ҁF�@HP��0�ɂȂ������̏���
// 2021/06/07 �X�V�ҁF����@�V�[���J�n���̃t�F�[�h�C���̍Œ��͓��͂��󂯕t���Ȃ�
// 2021/06/07 �X�V�ҁF����@�v���C���[���S���̃��X�|�[�������̒ǉ�
// 2021/07/05 �X�V�ҁF����@��q�̏�艺��@�\�̒ǉ�
// 2021/07/05 �X�V�ҁF����@���E���͂��󂯎�����ہA��x�ϐ��ɕۑ�����悤�ύX
// 2021/07/08 �X�V�ҁF����@��q��o��؂����ۂ̏�����ǉ�
// 2021/08/19 �X�V�ҁF��c�@�ړ��U�����̊�����ǉ�
// 2021/08/20 �X�V�ҁF�R�c�@�񕜃A�C�e�����擾������hp�𑝂₷����
// 2021/08/27 �X�V�ҁF�����@�W�����v�t���O���O������擾�ł���悤�ɍX�V
#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "BakemonoBakariCharacter.generated.h"

class ACameraControl;

//�߂��ɂ����q�̌��� 7/5����
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
	//�v���C���[���i�[�p�\���� 5/7
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

	//�X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

	/** Called for side to side input */
	void MoveRight(float Val);
	void MoveUp(float Val);

	// ���E�㉺���͂̒l���󂯎���ĕϐ��ɕۑ�����֐� 7/5����
	void InputRight(float Val);
	void InputUp(float Val);

	///** Handle touch inputs. */
	//void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	///** Handle touch stop event. */
	//void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//�_���[�W���󂯂鏈�� 5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void TakeDamage(float _dmg);

	//�񕜏��� 8/20 �R�c
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void TakeRecovery(float _recovery);

	//�_���[�W���󂯂���m�b�N�o�b�N���� 5/17
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void KnockBack(float _enemylocation);

	// ���A����
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStart();
public:
	// �f�o�b�O���[�h���̃t���O
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool IsDebug;

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
	//�E�����Ɍ����Ă��邩�t���O 5/10
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Direction")
		bool IsFaceRight;
	//��q��͂�ł��邩�t���O 7/5����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		bool IsLadder;
	//��q��o��؂����t���O 7/8����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		bool IsLadderClimb;
	//�߂��ɂ����q�̌��� 7/5����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ladder")
		ELadderDirection LadderDir;
	//���ċ����L�уt���O 5/14
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		bool IsHanging;
	//���G���ԃt���O 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Invicible")
		bool IsInvincible;
	//HP��0�ɂȂ��Ă��邩�t���O 5/26
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsDead;
	//�]�����S�t���O 5/28
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool IsFallDead;
	//�G�ƐڐG���Ă���t���O 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsEnemyContack;
	//�񕜃A�C�e���ƐڐG���Ă���t���O 8/20
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
		bool IsRecoveryContack;
	//�G�̐����ʒu 5/19
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float EnemyLocation;

	// ���E�㉺���͂̒l��ۑ�����ϐ� 7/5����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		FVector2D InputValue;
	
	//�t�F�[�h�C�����̓��͕s�t���O 6/7����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool IsInputFadeIn;

	//Overlap�e�X�g 5/14
	UPROPERTY(VisibleAnywhere, Category = "Overlap")
		class UCapsuleComponent* OverlapComponent;

	// �J����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	ACameraControl* m_pCamera;				

public:
	ABakemonoBakariCharacter();

	//Tick�֐�
	virtual void Tick(float DeltaTime) override;

	//�U�����͊֐� 4/23
	void Attack();

	//HP���擾�@5/7
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		int GetHP() { return m_info.hp; }

	// ���������̓��͒l��ێ� 8/27
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		float GetHorizontal() { return m_Horizontal; }

	//���ċ������L�т�֐� 5/14
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void Hang();

	// ��q��͂ނƂ��̏��� 7/5����
	void GraspLadder();

	// ��q����������Ƃ��̏��� 7/5����
	UFUNCTION(BlueprintCallable, Category = "Ladder")
		void LeaveLadder();

	// ��q�����؂����Ƃ��̏��� 7/5����
	void ClimbLadder(float DeltaTime);

	//�t�F�[�h�C�����̓��͕s�t���O��ݒ� 6/7����
	UFUNCTION(BlueprintCallable, Category = "Input")
		void SetInputFadeIn(bool b) { IsInputFadeIn = b; }

	//Overlap�֐� 5/14
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Overlap�֐� 5/14
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �W�����v���Ă��邩�ǂ���(����)
	bool GetIsJump() { return IsJump; }

private:
	void CheckLine();											// ���C���g���[�X��p���đ����ɒn�ʂ����邩���ׂ�

private:
	Player_Info m_info;						//�v���C���[���
	FVector m_ReStartPos;					// �v���C���[�̃��X�|�[�����W
	USkeletalMeshComponent* m_pMesh;		// ���b�V��
	float m_Horizontal;						//���������̓��͒l�ێ�
	bool m_IsGround;						// �ڒn���Ă��邩�ǂ���
};