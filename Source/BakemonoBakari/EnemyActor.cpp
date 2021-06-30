// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL
//			�F2021/5/17 �W�����v����G�̍s���v���O������ǉ�
//			�F2021/5/23 ���Ŏ��̉���ǉ��i����j
//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
//           
#include "Kismet/GameplayStatics.h"
#include "EnemyActor.h"
#include "CheckInScreen.h"

// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pPlayerCharacter(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_StartRote(FRotator::ZeroRotator)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O
	PrimaryActorTick.bCanEverTick = true;

	// �x�[�X�Ɋ֘A��
	//m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("m_pBase"));
	// bese��root�ɐݒ�
	//RootComponent = m_pBase;

	//	�R���W��������p�J�v�Z���R���|�[�l���g����
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

	// ���b�V����T��
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	// ��ʉ��ɂ��邩�𔻕ʂ���R���|�[�l���g�̐���
	m_pCheckInScreen = CreateDefaultSubobject<UCheckInScreen>(TEXT("CheckInScreen"));

	//m_pCheckInScreen = Cast<UCheckInScreen>(this);
}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// �����ʒu�̎擾
	m_initEnemyPosition = GetActorLocation();

	// ������]�̎擾
	m_StartRote = GetActorRotation();

	// HP�̏�����
	m_EnemyHP = m_EnemyHPMax;
}

// ���t���[���̏���
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐����L�^
	if (m_pEnemyMesh)
	{
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else 
	{
		// ���b�V����T��
		m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	}

	// ����ł�̂Ȃ珈�����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		Des();
		return;
	}

	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		Des();
		return;
	}
	else if (m_EnemyState != ENEMY_STATE_DESTROY)
	{
		m_IsInScreen = true;
		Indication();
	}

	// �X�e�[�^�X�̍X�V
	EnemyStatusControl(DeltaTime);
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
		if (OtherComp->ComponentHasTag("PlayerAttack"))
		{
			// �_���[�W��^����
			EnemyDamage();
		}

		// �ڐG�����A�N�^�[�̃^�O���v���C���[�ł����
		else if (m_pOverlappedActor->ActorHasTag("PlayerCharacter"))
		{
			//// �������~�߂�
			//EnemyStop();
		}
		// ���ɗ�����Ǝ��S
		else if (m_pOverlappedActor->ActorHasTag("Hole"))
		{
			m_EnemyState = ENEMY_STATE_DESTROY;
			Des();
		}
	}
	m_pOverlappedActor = NULL;
}

// �G�l�~�[�X�e�[�^�X�R���g���[��
void AEnemyActor::EnemyStatusControl(float _deltaTime)
{
	// ���S��
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		// ���S�A�j���[�V�������ԃJ�E���g
	}
	// �d����Ԃ̎�
	else if (m_EnemyState == ENEMY_STATE_STOP)
	{
		// �d���A�j���[�V�������ԃJ�E���g
	}
	// �_���[�W���
	else if (m_EnemyState == ENEMY_STATE_DAMAGE)
	{
		// �_���[�W�A�j���[�V�����J�E���g
		m_EnemyDamageAnimationCount += _deltaTime;

		if (m_EnemyDamageAnimationCount > m_DamageAnimationTime)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_EnemyDamageAnimationCount = 0.f;
			ChangeAnim();
		}
	}
	else if (m_EnemyState == ENEMY_STATE_ATTACK)
	{
		// �U���J�E���g
		m_EnemyAttackAnimationCount += _deltaTime;

		if (m_EnemyAttackAnimationCount > m_AttackAnimationTime)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_EnemyAttackAnimationCount = 0.f;
			ChangeAnim();
		}
	}
}

// �_���[�W�֐�
void AEnemyActor::EnemyDamage()
{
	// �_���[�W��Ԃ�������
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY)) return;

	m_EnemyState = ENEMY_STATE_DAMAGE;
	EnemyDamageEvent();

	//------------------------------------------------
	//hit�G�t�F�N�g����������
	//------------------------------------------------
	// HP�����炷�B�����̓v���C���[�̍U���l���Q�Ƃ���悤�ɂ��Ă���������
	--m_EnemyHP;
	m_EnemyState = ENEMY_STATE_DAMAGE;
	ChangeAnim();

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//���S�G�t�F�N�g����������
		//------------------------------------------------
		m_EnemyState = ENEMY_STATE_DESTROY;
		ChangeAnim();

		// �U�������o��
		if (m_crashSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_crashSound, GetActorLocation());
		}
		//Des();
	}
}

// ������
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);
}

// �A�j���[�V�����؂�ւ�
void AEnemyActor::ChangeAnim()
{
	// ���݂̃A�N�^�[�̏�Ԃ��r
	switch (m_EnemyState)
	{
	case AEnemyActor::ENEMY_STATE_NONE:
		break;

	case AEnemyActor::ENEMY_STATE_MOVE:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = true;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_IDLE:
		m_bIdling = true;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_DAMAGE:
		m_bIdling = false;
		m_bDamage = true;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_DESTROY:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = true;
		m_bStopping = false;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_STOP:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = true;
		m_bAttacking = false;
		break;

	case AEnemyActor::ENEMY_STATE_ATTACK:
		m_bIdling = false;
		m_bDamage = false;
		m_bMoving = false;
		m_bDestroy = false;
		m_bStopping = false;
		m_bAttacking = true;
		break;



	default:
		break;
	}
}
