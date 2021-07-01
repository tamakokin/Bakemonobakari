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

private:

	AActor* m_pActor;					// �v���C���[�ȊO�̃A�N�^�[

	// �ړ��ڕW�ɂ�����W
	float m_NowDistance;				// ���݃J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	float m_NowSpeed;					// �J�����̌��݂̈ړ����x

	// �O��̖ڕW���W
	FVector m_OldPos;

	// �v���C���[�̑O���ɃJ�������ړ�������
	FVector m_FrontPos;
	// �v���C���[�̒Ǐ]���s�����ǂ���
	bool m_Player;

	// �J�����ړ��̃��[�h�̐ؑ�
	float m_ChangeDistance;

	// �v���C���[�A�N�^
	AActor* m_pPlayerActor;

	// �J�������ړ������邩�ǂ���
	bool m_MoveHight;
	bool m_MoveWidth;

	// �J�����̗h��Ɏg�p����
	int m_shockCount;				// �h����	
	int m_shockTiming;				// �h���^�C�~���O
	float m_shockMaxWidth;			// ���̗h�ꕝ
	float m_shockMaxHeight;			// �c�̗h�ꕝ
	bool m_shockStart;				// �J������h�炷���ǂ���

public:
	// �J�����ړ��Ɏg�p����X�v���C���̍��W���擾����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		TArray<ACameraSpline*> m_pSpline;		// �X�v���C��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;		// �v���C���[��Ǐ]����ۂɒ����Ɏg��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_TargetPos;			// ���ڂ���A�N�^�[�̍��W

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;				// �J�����̏c�̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;				// �J�����̉��̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;				// �J������z�u���钍�ڃA�N�^�[����̉��s�̋���

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghWidth;				// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_LenghHight;				// ���݂̍��W�ƃv���C���[�Ƃ̋����������Ȃ�ړ�������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MaxPos;				// �J�������ړ��ł���͈͂̍ő�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MinPos;				// �J�������ړ��ł���͈͂̍ŏ��l

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

	// �h��̐ݒ���s���g�p����
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetShock(int _shockCount = 300, int _shockTiming = 10, float _shockMaxX = 200, float _shockMaxY = 200)
	{
		m_shockCount = _shockCount;
		m_shockTiming = _shockTiming;
		m_shockMaxWidth = _shockMaxX;
		m_shockMaxHeight = _shockMaxY;
		m_shockStart = true;
	}

	// �J�����̈ړ�����͈͂�ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetScope(FVector _max = FVector(2170.0f, 13480.0f, 550.0f),FVector _min = FVector(2170.0f, -13480.0f, -550.0f))
	{
		m_MaxPos = _max;
		m_MinPos = _min;
	}

	// �J�����̋�����ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetDistance(float _distance = 800.0f) { m_Distance = _distance; }

	// �����œn���ꂽ���W���J�����ɉf�邩�ǂ����𒲂ׂ�
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		bool CheckInCamera(FVector _pos,FVector _size = FVector::ZeroVector);

private:
	// �v���C���[����������
	void SearchPlayer();

	// �X�v���C����T��
	void SearchSpline();

	// �J�������͈͊O�ɂ���ꍇ�͈͂ɖ߂�
	void CheckInPos();

	// �J�������v���C���[�ɒǏ]������
	void MovePlayerCamera();

	// �J�����𒍖ڃA�N�^�[�Ɍ����Ĉړ�������(�J�����̌Œ�ȂǂɎg�p)
	void MoveCamera();

	// �J������h�炷
	void Shock();

	// �_A�Ɠ_B�̋������v��
	float Measurement(FVector _a, FVector _b);
};