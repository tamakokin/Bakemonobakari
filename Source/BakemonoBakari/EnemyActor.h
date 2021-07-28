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
//			�F2021/6/7  ���X�^�[�g���ɃG�l�~�[������������
//			�F			�G�l�~�[�����ꂽ�ꍇ��\���ɂ���

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

	// �R���C�_�[��\������
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void CollisionOff();

	// �R���C�_�[�\������
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void CollisionOn();

	// ���b�V���̕\������
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void MeshOn();

	// ���b�V���̔�\������
	UFUNCTION(BlueprintCallable, Category = "Enemy")
		void MeshOff();

	//// �q�b�g�G�t�F�N�g���o������
	//UFUNCTION(BlueprintCallable, Category = "Enemy")
	//	void Hit();

	// ����������
	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
		void ReStart();

	// �������W�Ɉړ�
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReStartPosition();

	// ��Ԃ̏�����
	UFUNCTION(BlueprintCallable, Category = "MyFunctions")
		void ReSetState();
private:
	// �I�[�o�[���b�v�֐�
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	// �X�e�[�^�X�R���g���[��
	void EnemyStatusControl(float _deltaTime);

	// �A�j���[�V�����ύX����
	void ChangeAnim();

	// �_���[�W���󂯂��ۂɓ_�ł�����
	void EnemyFlashing();

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

public:
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
	// �G�l�~�[�̃X�e�[�^�X���R���|�[�l���g�ɓ`����
	ENEMY_STATE GetEnmeyState()const { return m_EnemyState; }
	void SetEnemyState(const ENEMY_STATE _enemyState)
	{
		if ((m_EnemyState != ENEMY_STATE_DESTROY)&&(m_EnemyState != ENEMY_STATE_DAMAGE))
		{ 
			m_EnemyState = _enemyState; 
		} 
	}

	// �v���C���[���I�[�o�[���b�v�������Ƃ��R���|�[�l���g�ɓ`����
	AActor* GetOverlappedActor()const { return m_pOverlappedActor; }
	void SetOverlappedActor(AActor* _actor) { m_pOverlappedActor = _actor; }

	// �������W���R���|�[�l���g�ň���
	FVector GetStartPos()const { return m_initEnemyPosition; }

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

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 15))
		int m_EnemyHPMax;		// �G�̍ő�g�o

	int m_EnemyHP;				// �G�̌��݂�HP

	UPROPERTY(EditAnywhere)
		int m_score;			// �G��|�����ꍇ�Ɋl���ł���X�R�A

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_DamageAnimationTime;		// �_���[�W�̃A�j���[�V��������

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
		float m_AttackAnimationTime;		// �U���̃A�j���[�V��������

		bool m_IsInScreen;					// ��ʓ����邩�ǂ���

private:

	FVector m_initEnemyPosition;			// �����ʒu
	FRotator m_StartRote;					// �G�l�~�[�̏�����]

	// �A�j���[�V�����؂�ւ��p
	bool	m_bIdling;
	bool	m_bMoving;
	bool	m_bDamage;
	bool	m_bDestroy;
	bool	m_bStopping;
	bool	m_bAttacking;
	int	m_EnemyDamageCount;				// �_���[�W��̖��G���ԃJ�E���g�p

private:
	ENEMY_STATE m_EnemyState;			// �G�l�~�[�̃X�e�[�^�X
	AActor* m_pOverlappedActor;			// �I�[�o�[���b�v�����A�N�^�[
	AActor* m_pPlayerCharacter;

	UCheckInScreen* m_pCheckInScreen;
};