// 2021 05/01 ����I��
// �J�����̈ړ��������Ǘ�����

// 76�s�ڂɂɃX�N���[���̎~�ߕ�����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraControl.generated.h"

class ACameraSpline;

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

private:
	// �v���C���[����������
	void Search();

	// �X�v���C����T��
	void SearchSpline();

	// �J�������v���C���[�ɒǏ]������
	void MovePlayerCamera();

	// �J�����𒍖ڃA�N�^�[�Ɍ����Ĉړ�������(�J�����̌Œ�ȂǂɎg�p)
	void MoveCamera();

	// �Î~���A�ړ����̃J�����̊g�k���s��
	void ChangeScaleCamera(float _deltaTime);

	// �_A�Ɠ_B�̋������v��
	float Measurement(FVector _a, FVector _b);

public:
	// �J�����ړ��Ɏg�p����X�v���C���̍��W���擾����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TArray<ACameraSpline*> m_pSpline;		// �X�v���C��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;				// �v���C���[��Ǐ]����ۂɒ����Ɏg��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_TargetPos;					// ���ڂ���A�N�^�[�̍��W
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;						// �J�����̏c�̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;						// �J�����̉��̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "�g�呬�x", ClampMin = "0", ClampMax = "10"))
		float m_ScaleUpTime;					// �J�����̊g�呬�x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "�k�����x", ClampMin = "0", ClampMax = "10"))
		float m_ScaleDownTime;					// �J�����̏k�����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;						// �J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (DisplayName = "�g�嗦", ClampMin = "0", ClampMax = "200"))
		float m_Distance_ScaleUpMagnification;	// �ړ����Ɋg�傷��{��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghWidth;						// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghHight;						// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_MaxSpeed;						// �J�������ړ��ł���͈͂̍ő�l

private:

	AActor* m_pActor;							// �v���C���[�ȊO�̃A�N�^�[

	// �ړ��ڕW�ɂ�����W
	float m_NowDistance;						// ���݃J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	float m_NowSpeed;							// �J�����̌��݂̈ړ����x

	// �v���C���[�̑O���ɃJ�������ړ�������
	FVector m_FrontPos;
	// �v���C���[�̒Ǐ]���s�����ǂ���
	bool m_Player;

	// ���Ԃ��J�E���g
	float m_CountTime;

	// �v���C���[�A�N�^
	AActor* m_pPlayerActor;

	// �ړ����邩�ǂ���
	bool m_Move;
	bool m_PrevMove;

	// �E�Ɉړ����邩�ǂ���
	bool m_Right;
};