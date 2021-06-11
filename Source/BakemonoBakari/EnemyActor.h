// AEnemyActor.h
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O������ǉ�
//			�F2021/5/17 �G�f�B�^���œG�̎�ނ𕶎���ŕς�����悤�ɕύX
//			�F2021/5/23 ���Ŏ��̉���ǉ��i����j
//			�F2021/5/24 �l�X�ȓG�`�Ԃ�ǉ�
//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
//			�F2021/6/ 9 �A�j���[�V�����ɕK�v�ȗv�f�̒ǉ�


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnemyActor.generated.h"

class UCheckInScreen;

// �G�̎��
UENUM(BlueprintType)
enum class ENEMY_TYPE :uint8
{
	ENEMY_TYPE_NONE = 0,
	ENEMY_TYPE_STOP,			// �����Ȃ�
	ENEMY_TYPE_STRAIGHT,		// �܂������ړ�
	ENEMY_TYPE_JUMP,			// ���̏�ŃW�����v
	ENEMY_TYPE_JUMP_STRAIGHT,	// �ړ����Ȃ���W�����v
	ENEMY_TYPE_FIRING,			// �e����
	ENEMY_TYPE_FRY,				// ���
};

UCLASS()
class BAKEMONOBAKARI_API AEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	AEnemyActor();

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̏���
	virtual void Tick(float DeltaTime) override;

private:
	// �I�[�o�[���b�v�֐�
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	// �X�e�[�^�X�R���g���[��
	void EnemyStatusControl(float _deltaTime);

	// �ړ�����
	void EnemyMove(float _deltaTime);

	// �U������
	void EnemyAttack(float _deltaTime);

	// �A�j���[�V�����ύX����
	void ChangeAnim();

	// �v���C���[�ɓ����������̏���
	void EnemyStop();

public:
	UFUNCTION(BlueprintCallable, Category = "Enemy Function")
		// �_���[�W����
		void EnemyDamage();


// �A�j���[�V�����Ǘ��p
private:	
	// �A�C�h����Ԃ�
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsIdling() const { return m_bIdling; }

	// �����Ă��邩
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsMoving() const { return m_bMoving; }

	// �_���[�W���󂯂���
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDamage() const { return m_bDamage; }

	// �d��������
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsStopping() const { return m_bStopping; }

	// ���񂾂�
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsDestroy() const { return m_bDestroy; }

	// �U�����Ă��邩
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsAttacking() const { return m_bAttacking; }

private:

	// �G�l�~�[�̃X�e�[�^�X

	enum ENEMY_STATE
	{
		ENEMY_STATE_NONE = 0,
		ENEMY_STATE_MOVE,
		ENEMY_STATE_IDLE,
		ENEMY_STATE_ATTACK,
		ENEMY_STATE_DAMAGE,
		ENEMY_STATE_DESTROY,
		ENEMY_STATE_STOP,
	};

public:
	// blueprint EditAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* m_pCapsuleComp;		//	�^�[�Q�b�g�ڐG�p�R���W����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* m_pEnemyMesh;	// ���b�V��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* m_crashSound;			// ���Ŏ��̉�

private:
	UPROPERTY()
		USceneComponent* m_pBase;		// �x�[�X
	UPROPERTY(EditAnywhere)
		FName m_tagName;				// �^�O��

private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_moveSpeedY;		// �i�s�����̃X�s�[�h

	UPROPERTY(EditAnywhere)
		float m_moveRangeY;		// �i�s�����͈̔�

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 10.f))
		float m_InitVelocityZ;	// ������̏���

	UPROPERTY(EditAnywhere)
		float m_ChangeVectorTolerance;		// �v���C���[�ɓ��������ۂ̕����]���̋��e�ړ��͈�

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 29.4f))
		float m_JumpGravity;	// �d�͉����x

	UPROPERTY(EditAnywhere)
		float m_altitudeLimit;	// �ő卂�x

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 15))
		int m_EnemyHP;		// �G�̂g�o

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_JumpWait;		// �W�����v����܂ł̑ҋ@����

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackDelay;		// �U���܂ł̑ҋ@����

	UPROPERTY(EditAnywhere)
		ENEMY_TYPE m_EnemyType;	// �G�̎��


	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_DamageAnimationTime;		// �_���[�W�̃A�j���[�V��������

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackAnimationTime;		// �U���̃A�j���[�V��������




private:
	float	m_EnemyJumpDeltaTime;			// �W�����v���Ă��鎞��
	float	m_EnemyMovingDistance;			// �����Ă�������Ɉړ���������
	float	m_EnemyAttackingTime;			// �U��������̃f�B���C
	bool	m_StraightVector;				// �����Ă������
	bool	m_SwitchVector;					// �E���̂ǂ���������Ă��邩�̃t���O

	FVector m_initEnemyPosition;			// �����ʒu
	FVector m_prevEnemyPosition;			// 1�t���[���O�̎��g�̈ʒu

	// �A�j���[�V�����؂�ւ��p
	bool	m_bIdling;
	bool	m_bMoving;
	bool	m_bDamage;
	bool	m_bDestroy;
	bool	m_bStopping;
	bool	m_bAttacking;

	float	m_EnemyDamageAnimationCount;	// �_���[�W�A�j���[�V�����J�E���g�p
	float	m_EnemyAttackAnimationCount;	// �_���[�W�A�j���[�V�����J�E���g�p



private:
	ENEMY_STATE m_EnemyState;			// �G�l�~�[�̃X�e�[�^�X
	AActor* m_pOverlappedActor;			// �I�[�o�[���b�v�����A�N�^�[
	AActor* m_pPlayerCharacter;

	UCheckInScreen* m_pCheckInScreen;
};