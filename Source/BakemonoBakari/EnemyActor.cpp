// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL

#include "EnemyActor.h"

// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_EnemyType(ENEMY_TYPE_NONE)
	, m_pOverlappedActor(NULL)
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

	// ���͂����G�̎�ނ��v���p�e�B�ɔ��f
	m_EnemyType = (ENEMY_TYPE)m_EnemyType_Any;
	
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
		else if (m_pOverlappedActor->ActorHasTag("Player"))
		{
			// �������~�߂�
			// moveStop();
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

	switch (m_EnemyType)
	{

	case AEnemyActor::ENEMY_TYPE_NONE:		// �_�~�[
		break;

	case AEnemyActor::ENEMY_TYPE_STOP:		// �����Ȃ��G
		// �����Ȃ�
		break;

	case AEnemyActor::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G

		// �G�l�~�[�̈ʒu�����擾
		tempEnemyPosition.X += m_moveSpeedX;
		// �C����̃A�N�^�[�ʒu��ݒ�
		SetActorLocation(tempEnemyPosition);

		//---------------------------------------------------------------
		// �i��������������z����݂����ȃv���O�����������Ă���������
		//---------------------------------------------------------------
		break;

	case AEnemyActor::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G
		break;

	case AEnemyActor::ENEMY_TYPE_FRY:		// ����ԓG
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

	case AEnemyActor::ENEMY_TYPE_NONE:		// �_�~�[
		break;

	case AEnemyActor::ENEMY_TYPE_STOP:		// �����Ȃ��G
		break;

	case AEnemyActor::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G
		break;

	case AEnemyActor::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G
		break;

	case AEnemyActor::ENEMY_TYPE_FRY:		// ����ԓG
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
