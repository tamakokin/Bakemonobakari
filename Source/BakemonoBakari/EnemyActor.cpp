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
	, m_EnemyJumpDeltaTime(0.f)
	, m_EnemyMovingDistance(0.f)
	, m_EnemyAttackingTime(0.f)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_prevEnemyPosition(FVector::ZeroVector)
	, m_StraightVector(false)
	, m_SwitchVector(false)
	, m_Alive(true)
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
	if (!m_Alive)
	{
		m_EnemyState = ENEMY_STATE_IDLE;
		return;
	}
	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	if (!m_pCheckInScreen->Check(GetActorLocation())) 
	{ 
		return; 
	}
	
	// �ړ��֐�
	EnemyMove(DeltaTime);

	// �U������
	EnemyAttack(DeltaTime);
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
			// �������~�߂�
			EnemyStop();
		}
	}
	m_pOverlappedActor = NULL;
}

// �G�l�~�[�X�e�[�^�X�R���g���[��
void AEnemyActor::EnemyStatusControl(float _deltaTime)
{

}


// �ړ�����
void AEnemyActor::EnemyMove(float _deltaTime)
{
	// ��񂾂�
	if (m_pPlayerCharacter == NULL)
	{
		// �v���C���[��T��
		TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

		if (actors.Num() > 0)
		{
			for (int idx = 0; idx < actors.Num(); idx++)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);

				if (pActor->ActorHasTag("PlayerCharacter"))
				{
					UE_LOG(LogTemp, Warning, TEXT("Found Player"));
					m_pPlayerCharacter = pActor;
					break;
				}
			}
		}
	}

	// �_���[�W��Ԃ�������ړ����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	FVector tempEnemyPosition = GetActorLocation();
	FRotator tempEnemyRotation = GetActorRotation();

	FVector tempPlayerCharacterPosition = FVector::ZeroVector;
	if (m_pPlayerCharacter != NULL)
	{
		tempPlayerCharacterPosition = m_pPlayerCharacter->GetActorLocation();
	}

	// �ψ�Z
	float tempDeltaZ;

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

		m_EnemyMovingDistance += m_moveSpeedY;

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

		//UE_LOG(LogTemp, Warning, TEXT("jumping"));

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

	case ENEMY_TYPE::ENEMY_TYPE_JUMP_STRAIGHT:
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

		// ���Ԃ��J�E���g
		m_EnemyJumpDeltaTime += _deltaTime;

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


	case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// �e���˓G

		if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
		{
			// �O����
			tempEnemyRotation.Yaw = 90.f;
			m_StraightVector = false;
		}
		else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
		{
			// ������
			tempEnemyRotation.Yaw = 270.f;
			m_StraightVector = true;
		}
		SetActorRotation(tempEnemyRotation);

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
void AEnemyActor::EnemyAttack(float _deltaTime)
{
	// �_���[�W��Ԃ�������U�����Ȃ�
	if (m_EnemyState == ENEMY_STATE_DAMAGE) return;

	// �A�N�^�[�����p
	FString BulletPath;
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor;
	m_EnemyAttackingTime += _deltaTime;

	//---------------------------------------------------------------
	// �U�����@���s���̂��ߏ�������̂݋L�ڂ��܂�
	//---------------------------------------------------------------
	switch (m_EnemyType)
	{

	case ENEMY_TYPE::ENEMY_TYPE_NONE:		// �_�~�[
		m_EnemyAttackingTime = 0.f;
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STOP:		// �����Ȃ��G
		m_EnemyAttackingTime = 0.f;
		break;

	case ENEMY_TYPE::ENEMY_TYPE_STRAIGHT:	// �^�������˂�����ł���G
		m_EnemyAttackingTime = 0.f;
		break;

	case ENEMY_TYPE::ENEMY_TYPE_JUMP:		// �W�����v���Ă���G
		m_EnemyAttackingTime = 0.f;
		break;

	case ENEMY_TYPE::ENEMY_TYPE_FIRING:		// �e����
		if (m_EnemyAttackingTime > m_AttackDelay)
		{

			// BP������p�X���w��
			BulletPath = "/Game/Blueprints/EnemyBulletBP.EnemyBulletBP_c";

			// �p�X�w�肵��BP�̃A�N�^�[�N���X���i�[
			subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

			// �ʒu�Ɖ�]��ݒ�
			location = GetActorLocation(); //+ m_pCapsuleComp->GetRightVector() * 10;
			rotation = GetActorRotation();

			if (!m_StraightVector)
			{
				rotation.Yaw = 0.f;
			}
			else
			{
				rotation.Yaw = 180.f;
			}
			// �ݒ肵���l�𔽉f���ăX�|�[��
			actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

			m_EnemyAttackingTime = 0.f;
		}
		break;

	case ENEMY_TYPE::ENEMY_TYPE_FRY:		// ����ԓG
		m_EnemyAttackingTime = 0.f;
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
		if (m_EnemyMovingDistance > m_ChangeVectorTolerance)
		{
			m_SwitchVector = true;
			m_StraightVector = !m_StraightVector;
		}
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

	EnemyDamageEvent();

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

		// �U�������o��
		if (m_crashSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_crashSound, GetActorLocation());
		}

		m_Alive = false;
		Des();
	}
}

// ������
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyJumpDeltaTime = 0.0f;
	m_EnemyMovingDistance = 0.0f;
	m_EnemyAttackingTime = 0.0f;
	m_StraightVector = false;
	m_SwitchVector = false;
	m_Alive = true;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);
}
