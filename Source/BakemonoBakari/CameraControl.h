// 2021 05/01 ����I�� �J�����̈ړ��������Ǘ�����
// 2021/08/25 �����C�l �Î~���ɃJ�����������A�ړ����ɃJ�������߂Â���悤�ɍX�V
// 2021/08/27 �����C�l �v���C���[�L�����N�^�[�̌^��ύX�A�X�v���C���ɉ����Ĉړ����邩�ǂ����̃t���O��ǉ�
// 2021/08/28 �R���g���[���[����̓��͂ŃJ�������ړ��ł���悤�ɂ���

// 76�s�ڂɂɃX�N���[���̎~�ߕ�����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BakemonoBakariCharacter.h"
#include "CameraControl.generated.h"

class ACameraSpline;
class ABakemonoBakariCharacter;
UCLASS()
class BAKEMONOBAKARI_API ACameraControl : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// �p�����[�^���O����蒲���i�J�����̃X�N���[�����~�߂����Ƃ��̓J������~�ʒu�̃A�N�^�[�������ɓ���Ă�����Ăԁj

	// �J�����̈ړ�������ۂɒǏ]����A�N�^�[�A�ړ����x�ɐݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetActor(AActor* _targtActor, float _speed = 20.0f)
	{
		m_pActor = _targtActor;
		m_NowSpeed = _speed;
		m_Player = false;
	}

	// �v���C���[��Ǐ]����悤�ɐݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetPlayerActor()
	{
		m_NowSpeed = m_SpeedWidth;
		m_Player = true;
	}

	// �J�����̋�����ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetDistance(float _distance = 800.0f) { m_Distance = _distance; }

	// �����œn���ꂽ���W���J�����ɉf�邩�ǂ����𒲂ׂ�
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		bool CheckInCamera(FVector _pos, FVector _size = FVector::ZeroVector);

	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetMoveCameraY(float _move) { m_MoveCamera.Y += _move; }

	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetMoveCameraZ(float _move) { m_MoveCamera.Z += _move; }

	// �X�v���C����T��
	void SearchSpline();

private:
	// �v���C���[����������
	void Search();

	// �J�������v���C���[�ɒǏ]������
	void MovePlayerCamera(float _deltaTime);

	// �J�����𒍖ڃA�N�^�[�Ɍ����Ĉړ�������(�J�����̌Œ�ȂǂɎg�p)
	void MoveCamera();

	// �_A�Ɠ_B�̋������v��
	float Measurement(FVector _a, FVector _b);

public:
	// �J�����ړ��Ɏg�p����X�v���C���̍��W���擾����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TArray<ACameraSpline*> m_pSpline;		// �X�v���C��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Position")
		FVector m_AdjustmentPos;				// �v���C���[��Ǐ]����ۂɒ����Ɏg��

	UPROPERTY(VisibleAnywhere, Category = "Camera|Position")
		FVector m_TargetPos;					// ���ڂ���A�N�^�[�̍��W

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Position")
		FVector m_CameraMoveReturn;				// �J�������ړ�����ۂ̌��E�l
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_SpeedHight;						// �J�����̏c�̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_SpeedWidth;						// �J�����̉��̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "10"))
		float m_ScaleUpTime;					// �J�����̊g�呬�x�i�����j
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "10"))
		float m_ScaleDownTime;					// �J�����̏k�����x�i�����j
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;						// �J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Scale", meta = (ClampMin = "0", ClampMax = "200"))
		float m_Distance_ScaleUpMagnification;	// �ړ����Ɋg�傷��{���i�����j

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Length")
		float m_LenghWidth;						// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Length")
		float m_LenghHight;						// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float m_MaxSpeed;						// �J�������ړ��ł���͈͂̍ő�l

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float  m_SwayingMax;					// �v���C���[���n�ʂɓ����������ɗh�炷�����l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Speed")
		float  m_SwayingAdd;					// �v���C���[���n�ʂɓ����������ɗh�炷���݂̗h��̑�����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayPriority = "1"))
		bool m_bSplineMode;						// �X�v���C���ɉ����Ĉړ����邩�ǂ����i�����j

private:

	AActor* m_pActor;							// �v���C���[�ȊO�̃A�N�^�[

	// �ړ��ڕW�ɂ�����W
	float m_NowDistance;						// ���݃J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	float m_NowSpeed;							// �J�����̌��݂̈ړ����x

	// �v���C���[���n�ʂɓ����������ɗh�炷���݂̗h��̑���
	float  m_SwayingNow;
	// �v���C���[���n�ʂɓ����������ɗh����N�������ǂ���
	bool m_IsSway;

	// �v���C���[�̑O���ɃJ�������ړ�������
	FVector m_FrontPos;

	// ���͂ɂ���ăJ�������ړ�������
	FVector m_MoveCamera;

	ABakemonoBakariCharacter* m_pPlayerActor;

	// �v���C���[�̒Ǐ]���s�����ǂ���
	bool m_Player;

	// ���Ԃ��J�E���g
	float m_CountTime;
	bool m_bCount;
	bool m_Move;
	bool m_PrevMove;

	// �E�Ɉړ����邩�ǂ���
	bool m_Right;

	// �؂�ւ���O�̃J�����̍��W
	FVector m_PrevChangeCameraPos;
	// �ЂƂO�̃J�����̍��W
	FVector m_PrevCameraPos;
	// ��O�̃W�����v�t���O
	bool m_PrevIsJump;
};