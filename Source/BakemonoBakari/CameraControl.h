// 2021 05/01 ����I��
// �J�����̈ړ��������Ǘ�����

// 76�s�ڂɂɃX�N���[���̎~�ߕ�����
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraControl.generated.h"

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

	FVector m_TargetPos;

	// �O��̖ڕW���W
	FVector m_OldPos;

	// �v���C���[�̒Ǐ]���s�����ǂ���
	bool m_Player;

	// �J�����̗h��Ɏg�p����
	int m_shockCount;				// �h����	
	int m_shockTiming;				// �h���^�C�~���O
	float m_shockMaxWidth;			// ���̗h�ꕝ
	float m_shockMaxHeight;			// �c�̗h�ꕝ
	bool m_shockStart;				// �J������h�炷���ǂ���

public:
	// �J���������ڂ���A�N�^�[���擾
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		AActor* m_pPlayerActor;				// �v���C���[

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_AdjustmentPos;		// �v���C���[��Ǐ]����ۂɒ����Ɏg��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedHight;				// �J�����̏c�̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_SpeedWidth;				// �J�����̉��̈ړ����x
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Distance;				// �J������z�u���钍�ڃA�N�^�[����̉��s�̋���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float m_Lengh;					// ���݂̍��W�ƖڕW���W�Ƃ̋������Z���Ȃ�ړ������Ȃ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MaxPos;				// �J�������ړ��ł���͈͂̍ő�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		FVector m_MinPos;				// �J�������ړ��ł���͈͂̍ŏ��l
private:
	// �v���C���[����������
	void SearchPlayer();

public:
	// �p�����[�^���O����蒲���i�J�����̃X�N���[�����~�߂����Ƃ��̓J������~�ʒu�̃A�N�^�[�������ɓ���Ă�����Ăԁj

	// ���ڂ���A�N�^�[�A�ړ����x�ɐݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetActor(AActor* _targtActor, float _speed)
	{
		m_pActor = _targtActor;
		m_NowSpeed = _speed;
		m_Player = false;
	};
	// �v���C���[�𒍖ڃA�N�^�[�ɐݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetTargetPlayerActor()
	{
		m_pActor = m_pPlayerActor;
		m_NowSpeed = m_SpeedWidth;
		m_Player = true;
	};
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
		void SetScope(FVector _max, FVector _min)
	{
		m_MaxPos = _max;
		m_MinPos = _min;
	}

	// �J�����̋�����ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		void SetDistance(float _distance) { m_Distance = _distance; }


	// �����œn���ꂽ���W���J�����ɉf�邩�ǂ����𒲂ׂ�
	UFUNCTION(BlueprintCallable, Category = "Camera Function")
		bool CheckInCamera(FVector _pos);

private:
	// �J�������͈͊O�ɂ���ꍇ�͈͂ɖ߂�
	void CheckInPos();

	// �v���C���[�̌����ɂ���Ē��S���W��ύX
	void NoticePlayer();

	// �J�������v���C���[�ɒǏ]������
	void MovePlayerCamera();

	// �J�����𒍖ڃA�N�^�[�Ɍ����Ĉړ�������ړ�������
	void MoveCamera();

	// �J������h�炷
	void Shock();

	// �_A�Ɠ_B�̋������v��
	float Measurement(FVector _a, FVector _b);
};