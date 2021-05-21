// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL
//			�F2021/5/17 �W�����v����G�̍s���v���O������ǉ�

#include "EnemyActor.h"

// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_EnemyJumpDeltaTime(0.f)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_prevEnemyPosition(FVector::ZeroVector)
	, m_StraightVector(false)
	, m_SwitchVector(false)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O
	PrimaryActorTick.bCanEverTick = true;

	// �x�[�X�Ɋ֘A��
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("m_pBase"));
	// bese��root�ɐݒ�
	RootComponent = m_pBase;

	// ���b�V����T��
	m_pEnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("m_pEnemyMesh"));
	// ���b�V��������
	m_pEnemyMesh->SetupAttachment(RootComponent);

	//	�R���W��������p�J�v�Z���R���|�[�l���g����
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// �����ʒu�̎擾
	m_initEnemyPosition = GetActorLocation();
}

// ���t���[���̏���
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐����L�^
	if (m_pCapsuleComp != NULL)
	{
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}

	// �ړ��֐�
	EnemyMove(DeltaTime);
}

// �I�[�o�[���b�v�֐�
UFUNCTION() void AEnemyActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �I�[�o�[���b�v�����A�N�^�[���i�[
	m_pOverlappedActor = OtherActor;

	// �A�N�^�[�̃|�C���^�[���ʂ�łȂ����
	if (m_pOverlappedActor != NULL)
	{
		// �ڐG�����A�N�^�[�̃^�O���v���C���[�̍U���ł����
		if (m_pOverlappedActor->ActorHasTag("PlayerAttack"))
		{
			// �_���[�W��^����
			EnemyDamage();
		}

		// �ڐG�����A�N�^�[�̃^�O���v���C���[�ł����
		else if (m_pOverlappedActor->ActorHasTag("PlayerCharacter"))
		{
			// �������~�߂�
			EnemyStop();
		}
	}
	m_pOverlappedActor = NULL;
}


// �ړ�����
void AEnemyActor::EnemyMove(float _deltaTime)
{
	// �_���[�W��Ԃ�������ړ����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	FVector tempEnemyPosition = GetActorLocation();
	FRotator tempEnemyRotation = GetActorRotation();

	switch (m_EnemyType)
	{

	case ENEMY_TYPE::ENEMY_TYPE_NONE:		// �_�~�[
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STOP:		// �����Ȃ��G
		// �����Ȃ�
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G

		// �����]��
		if (m_SwitchVector)
		{
			if (!m_StraightVector)
			{
				// �O����
				tempEnemyRotation.Yaw = 90.f;
			}
			else
			{
				// ������
				tempEnemyRotation.Yaw = 270.f;
			}

			m_SwitchVector = false;
			SetActorRotation(tempEnemyRotation);
		}

		m_prevEnemyPosition = tempEnemyPosition;

		// �i�ޕ����ɉ�����
		if (!m_StraightVector)
		{
			// �O�i
			tempEnemyPosition.Y += m_moveSpeedY;
		}
		else
		{
			// ���
			tempEnemyPosition.Y -= m_moveSpeedY;
		}

		// �C����̃A�N�^�[�ʒu��ݒ�
		SetActorLocation(tempEnemyPosition);

		// ���͈͂𒴂�����
		if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
		{
			m_SwitchVector = true;
			m_StraightVector = !m_StraightVector;
		}

		//---------------------------------------------------------------
		// �i��������������z����݂����ȃv���O�����������Ă���������
		//---------------------------------------------------------------
		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G

		// ���Ԃ��J�E���g
		m_EnemyJumpDeltaTime += _deltaTime;

		// �ҋ@���Ԃ𒴂���܂œ����Ȃ�
		if (m_EnemyJumpDeltaTime < m_JumpWait)	return;

		UE_LOG(LogTemp, Warning, TEXT("jumping"));

		// �ψ�Y
		float tempDeltaZ;

		// ���������グ
		tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
		tempEnemyPosition.Z += tempDeltaZ;

		// �����ʒu��艺�ɍs�����ꍇ�␳���ă��^�[��
		if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
		{
			// �����ʒu�̖߂����玞�Ԃ̏�����
			SetActorLocation(tempEnemyPosition);
			m_EnemyJumpDeltaTime = 0.f;
			return;
		}

		// �l�̔��f
		SetActorLocation(tempEnemyPosition);

		break;

	case ENEMY_TYPE::ENEMY_TYPE_FRY:		// ����ԓG
		//---------------------------------------------------------------
		// ����ɂ���ď��������܂�
		//---------------------------------------------------------------
		break;

	default:
		break;
	}
}

// �U������
void AEnemyActor::EnemyAttack()
{
	// �_���[�W��Ԃ�������U�����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	//---------------------------------------------------------------
	// �U�����@���s���̂��ߏ�������̂݋L�ڂ��܂�
	//---------------------------------------------------------------
	switch (m_EnemyType)
	{

	case ENEMY_TYPE::ENEMY_TYPE_NONE:		// �_�~�[
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STOP:		// �����Ȃ��G
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G
		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G
		break;

	case ENEMY_TYPE::ENEMY_TYPE_FRY:		// ����ԓG
		break;

	default:
		break;
	}
}

// �v���C���[�ɓ����������̏���
void AEnemyActor::EnemyStop()
{
	switch (m_EnemyType)
	{
	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G
		// �����]��
		m_SwitchVector = true;
		m_StraightVector = !m_StraightVector;
		break;

	default:
		break;
	}
}

// �_���[�W�֐�
void AEnemyActor::EnemyDamage()
{
	// �_���[�W��Ԃ�������
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	if (m_EnemyHP > 0)
	{
		//------------------------------------------------
		//hit�G�t�F�N�g����������
		//------------------------------------------------
		// HP�����炷�B�����̓v���C���[�̍U���l���Q�Ƃ���悤�ɂ��Ă���������
		--m_EnemyHP;
	}

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//���S�G�t�F�N�g����������
		//------------------------------------------------
		Destroy();
	}
}