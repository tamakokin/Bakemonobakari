// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL
//			�F2021/5/17 �W�����v����G�̍s���v���O������ǉ�
//			�F2021/5/23 ���Ŏ��̉���ǉ��i����j
//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j

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
	if (m_pCapsuleComp != NULL)
	{
		m_pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	// ����ł�̂Ȃ珈�����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DESTROY)
	{
		return;
	}
	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	if (!m_pCheckInScreen->Check(GetActorLocation())) 
	{ 
		Des();
		return; 
	}
	else 
	{
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


// �ړ�����
void AEnemyActor::EnemyMove(float _deltaTime)
{
	// �_���[�W��Ԃ�������ړ����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	FVector tempEnemyPosition = GetActorLocation();
	FRotator tempEnemyRotation = GetActorRotation();

	ChangeAnim();

	//switch (m_EnemyType)
	//{

	//case ENEMY_TYPE::ENEMY_TYPE_NONE:		// �_�~�[
	//	break;

	//case ENEMY_TYPE::ENEMY_TYPE_STOP:		// �����Ȃ��G
	//	// �����Ȃ�
	//	break;

	////case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G

	////	// �����]��
	////	if (m_SwitchVector)
	////	{
	////		if (!m_StraightVector)
	////		{
	////			// �O����
	////			tempEnemyRotation.Yaw = 90.f;
	////		}
	////		else
	////		{
	////			// ������
	////			tempEnemyRotation.Yaw = 270.f;
	////		}

	////		m_SwitchVector = false;
	////		SetActorRotation(tempEnemyRotation);
	////	}

	////	m_prevEnemyPosition = tempEnemyPosition;

	////	// �i�ޕ����ɉ�����
	////	if (!m_StraightVector)
	////	{
	////		// �O�i
	////		tempEnemyPosition.Y += m_moveSpeedY;
	////	}
	////	else
	////	{
	////		// ���
	////		tempEnemyPosition.Y -= m_moveSpeedY;
	////	}

	////	m_EnemyMovingDistance += m_moveSpeedY;

	////	// �C����̃A�N�^�[�ʒu��ݒ�
	////	SetActorLocation(tempEnemyPosition);

	////	// ���͈͂𒴂�����
	////	if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
	////	{
	////		m_SwitchVector = true;
	////		m_StraightVector = !m_StraightVector;
	////	}

	////	m_EnemyState = ENEMY_STATE_MOVE;
	////	ChangeAnim();

	////	//---------------------------------------------------------------
	////	// �i��������������z����݂����ȃv���O�����������Ă���������
	////	//---------------------------------------------------------------
	////	break;

	////case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G

	////	// ���Ԃ��J�E���g
	////	m_EnemyJumpDeltaTime += _deltaTime;

	////	// �ҋ@���Ԃ𒴂���܂œ����Ȃ�
	////	if (m_EnemyJumpDeltaTime < m_JumpWait)	return;

	////	//UE_LOG(LogTemp, Warning, TEXT("jumping"));

	////	// ���������グ
	////	tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
	////	tempEnemyPosition.Z += tempDeltaZ;

	////	// �����ʒu��艺�ɍs�����ꍇ�␳���ă��^�[��
	////	if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
	////	{
	////		// �����ʒu�̖߂����玞�Ԃ̏�����
	////		SetActorLocation(tempEnemyPosition);
	////		m_EnemyJumpDeltaTime = 0.f;
	////		return;
	////	}

	////	// �l�̔��f
	////	SetActorLocation(tempEnemyPosition);

	////	m_EnemyState = ENEMY_STATE_MOVE;
	////	ChangeAnim();

	////	break;

	//case ENEMY_TYPE::ENEMY_TYPE_JUMP_STRAIGHT:
	//	//// �����]��
	//	////if (m_SwitchVector)
	//	//{
	//	//	if (!m_StraightVector)
	//	//	{
	//	//		// �O����
	//	//		tempEnemyRotation.Yaw = 90.f;
	//	//	}
	//	//	else
	//	//	{
	//	//		// ������
	//	//		tempEnemyRotation.Yaw = 270.f;
	//	//	}

	//	//	//m_SwitchVector = false;
	//	//	SetActorRotation(tempEnemyRotation);
	//	//}

	//	//// �i�ޕ����ɉ�����
	//	//if (!m_StraightVector)
	//	//{
	//	//	// �O�i
	//	//	tempEnemyPosition.Y += m_moveSpeedY;
	//	//}
	//	//else
	//	//{
	//	//	// ���
	//	//	tempEnemyPosition.Y -= m_moveSpeedY;
	//	//}

	//	//// �C����̃A�N�^�[�ʒu��ݒ�
	//	//SetActorLocation(tempEnemyPosition);

	//	//// ���͈͂𒴂�����
	//	//if (tempEnemyPosition.Y > (m_initEnemyPosition.Y + m_moveRangeY) || tempEnemyPosition.Y < m_initEnemyPosition.Y)
	//	//{
	//	//	//m_SwitchVector = true;
	//	//	m_StraightVector = !m_StraightVector;
	//	//}

	//	//// ���Ԃ��J�E���g
	//	//m_EnemyJumpDeltaTime += _deltaTime;

	//	//// ���������グ
	//	//tempDeltaZ = (m_InitVelocityZ - 0.5 * m_JumpGravity * ((m_EnemyJumpDeltaTime - m_JumpWait) * (m_EnemyJumpDeltaTime - m_JumpWait)));
	//	//tempEnemyPosition.Z += tempDeltaZ;

	//	//// �����ʒu��艺�ɍs�����ꍇ�␳���ă��^�[��
	//	//if (tempEnemyPosition.Z < m_initEnemyPosition.Z)
	//	//{
	//	//	// �����ʒu�̖߂����玞�Ԃ̏�����
	//	//	SetActorLocation(tempEnemyPosition);
	//	//	m_EnemyJumpDeltaTime = 0.f;

	//	//	m_EnemyState = ENEMY_STATE_MOVE;
	//	//	ChangeAnim();
	//	//	return;
	//	//}

	//	//// �l�̔��f
	//	//SetActorLocation(tempEnemyPosition);
	//	//m_EnemyState = ENEMY_STATE_MOVE;
	//	ChangeAnim();

	//	//break;


	//case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// �e���˓G

	//	if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
	//	{
	//		// �O����
	//		tempEnemyRotation.Yaw = 90.f;
	//		m_StraightVector = false;
	//	}
	//	else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
	//	{
	//		// ������
	//		tempEnemyRotation.Yaw = 270.f;
	//		m_StraightVector = true;
	//	}
	//	SetActorRotation(tempEnemyRotation);

	//	break;

	//case ENEMY_TYPE::ENEMY_TYPE_FRY:		// ����ԓG
	//	//---------------------------------------------------------------
	//	// ����ɂ���ď��������܂�
	//	//---------------------------------------------------------------
	//	break;

	//default:
	//	break;
}

//// �v���C���[�ɓ����������̏���
//void AEnemyActor::EnemyStop()
//{
//	switch (m_EnemyType)
//	{
//	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G
//		// �����]��
//		if (m_EnemyMovingDistance > m_ChangeVectorTolerance)
//		{
//			//m_SwitchVector = true;
//			m_StraightVector = !m_StraightVector;
//		}
//		break;
//
//	default:
//		break;
//	}
//}

// �_���[�W�֐�
void AEnemyActor::EnemyDamage()
{
	// �_���[�W��Ԃ�������
	if ((m_EnemyState == ENEMY_STATE_DAMAGE)||(m_EnemyState == ENEMY_STATE_DESTROY)) return;

	m_EnemyState = ENEMY_STATE_DAMAGE;
	EnemyDamageEvent();

	if (m_EnemyHP > 0)
	{
		//------------------------------------------------
		//hit�G�t�F�N�g����������
		//------------------------------------------------
		// HP�����炷�B�����̓v���C���[�̍U���l���Q�Ƃ���悤�ɂ��Ă���������
		--m_EnemyHP;
		m_EnemyState = ENEMY_STATE_DAMAGE;
		ChangeAnim();
	}

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
		Des();
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
