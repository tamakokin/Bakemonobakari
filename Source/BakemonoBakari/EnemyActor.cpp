// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL
//			�F2021/5/17 �W�����v����G�̍s���v���O������ǉ�
//			�F2021/5/23 ���Ŏ��̉���ǉ��i����j
//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
//			�F2021/8/17 �|�����������ǂ����ŗ^�_���[�W����؂�ւ���悤�Ɂi����j
//			�F2021/9/ 7 �|�����ۂ�UI���b�Z�[�W���o�� 

#include "EnemyActor.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CheckInScreen.h"
#include "Enemy_Rote_Component.h"
#include "Materials/MaterialInstanceDynamic.h"

#define RAD_COLOR 0.3
#define DAMAGE_ANIME_END 30
#define FLASH_END 10
#define FLASH_TIMING 5
// Sets default values
AEnemyActor::AEnemyActor()
	: m_pEnemyMesh(NULL)
	, m_pCapsuleComp(NULL)
	, m_pPlayerCharacter(NULL)
	, m_pBase(NULL)
	, m_EnemyState(ENEMY_STATE_IDLE)
	, m_pOverlappedActor(NULL)
	, m_score(500.f)
	, m_initEnemyPosition(FVector::ZeroVector)
	, m_StartRote(FRotator::ZeroRotator)
	, m_DamageEndTiming(100)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O
	PrimaryActorTick.bCanEverTick = true;

	//	�R���W��������p�J�v�Z���R���|�[�l���g����
	m_pCapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	m_pCapsuleComp->SetupAttachment(RootComponent);

	// ���b�V����T��
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));


	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	// ��ʉ��ɂ��邩�𔻕ʂ���R���|�[�l���g�̐���
	m_pCheckInScreen = CreateDefaultSubobject<UCheckInScreen>(TEXT("CheckInScreen"));
	
	// �����]�����s���R���|�[�l���g�̐���
	m_pEnemyRote = Cast<UEnemy_Rote_Component>(GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// �R���|�[�l���g�̎擾
	if (!m_pEnemyRote) 
	{
		m_pEnemyRote = Cast<UEnemy_Rote_Component>(GetComponentByClass(UEnemy_Rote_Component::StaticClass()));
	}


	// �����ʒu�̎擾
	m_initEnemyPosition = GetActorLocation();

	// ������]�̎擾
	m_StartRote = GetActorRotation();

	// HP�̏�����
	m_EnemyHP = m_EnemyHPMax;

	// �s���\��Ԃɂ���
	m_IsAction = true;

	//	�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐����L�^
	if (m_pEnemyMesh)
	{
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
	else
	{
		// ���b�V����T��
		m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlapBegin);
	}
}

// ���t���[���̏��� ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		// �����蔻��̖�����
		CollisionOff();
		// ���b�V���̖�����
		MeshOff();

		// �s���ł��Ȃ��悤�ɂ���
		m_IsAction = false;
		return;
	}
	else if ((m_EnemyState != ENEMY_STATE_DESTROY) && (m_EnemyDamageCount <= 0))
	{
		// �s���\��Ԃɂ���
		m_IsAction = true;
		m_IsInScreen = true;
		MeshOn();
		CollisionOn();
	}

	// ���G���ԂȂ�_�ł�����
	if (m_EnemyDamageCount > 0)
	{
		EnemyFlashing();
	}

	// �X�e�[�^�X�̍X�V
	ChangeAnim();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �I�[�o�[���b�v�֐�----------------------------------------------------------------------------------------------------------------------------------------------------
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
		// ���ɗ�����Ǝ��S
		else if (m_pOverlappedActor->ActorHasTag("Hole"))
		{
			m_EnemyState = ENEMY_STATE_DESTROY;
		}
	}
	m_pOverlappedActor = NULL;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �_���[�W�֐�-------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::EnemyDamage()
{
	// ���G��ԂȂ�
	if (m_EnemyDamageCount > 0)return;

	// �s���s�\��Ԃɂ���
	m_IsAction = false;

	// ���G��Ԃɂ���
	m_EnemyDamageCount = 1;

	// �_���[�W���󂯂���Ԃɂ���
	m_EnemyState = ENEMY_STATE_DAMAGE;

	// �ԐF�ɂ���
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(RAD_COLOR, 0.0f, 0.0f));

	// �q�b�g�G�t�F�N�g���o��
	Hit();

	// HP�����炷�B�����̓v���C���[�̍U���l���Q�Ƃ���悤�ɂ��Ă���������
	--m_EnemyHP;


	if (m_EnemyHP <= 0)
	{
		Des();
	}
	else
	{
		// �^�_���[�W�����o��
		if (m_EnemyDamageSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_EnemyDamageSound, GetActorLocation());
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���S����------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::Des() 
{
	if (!m_pEnemyRote)return;

	// ���S�A�j���[�V�����Đ�
	m_EnemyState = ENEMY_STATE_DESTROY;
	ChangeAnim();

	// �s���s�\�ɂ���
	m_IsAction = false;

	// �v���C���[�̕���������
	m_pEnemyRote->EnemyRote();

	// ������悤�ɂ���@
	m_pEnemyMesh->SetVisibility(true);

	// �X�R�A�����Z����
	Cast<UMyGameInstance>(GetGameInstance())->AddScore(m_score, SCORE_TYPE::SCORE_TYPE_NORMAL_ENEMY);
	
	// �X�R�AUI���o��������
	//if (m_pEnemyScore != NULL)
	{
		AEnemyScore* m_pEnemyScore;
		FString ScorePath = "/Game/Blueprints/EnemyScore.EnemyScore_C";
		TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(ScorePath)).LoadSynchronous();

		m_pEnemyScore = GetWorld()->SpawnActor<AEnemyScore>(subClass, GetActorLocation(), FRotator(0.f, 180.f, 0.f));
		m_pEnemyScore->SetScore(m_score);
	}

	// �|�������������o��
	if (m_EnemyLethalDamageSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, m_EnemyLethalDamageSound, GetActorLocation());
	}

	// �����蔻��̖�����
	CollisionOff();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ���G���̃}�e���A���̕ω�------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���G���ԊJ�n
void AEnemyActor::EnemyFlashing()
{
	m_EnemyDamageCount++;

	// �ԐF�����Ƃɖ߂�
	if (m_EnemyDamageCount >= FLASH_END)
	{
		// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	}

	if (m_EnemyState != ENEMY_STATE_DESTROY)
	{
		// �_���[�W�A�j���[�V�������I��
		if (m_EnemyDamageCount == DAMAGE_ANIME_END)
		{
			m_EnemyState = ENEMY_STATE_IDLE;
			m_IsAction = true;
		}

		// ���G���ɂ͓_�ł�����
		if (m_EnemyDamageCount % (FLASH_TIMING *2) == 0)
		{
			m_pEnemyMesh->SetVisibility(false);
		}
		else if (m_EnemyDamageCount % FLASH_TIMING == 0)
		{
			m_pEnemyMesh->SetVisibility(true);
		}

		// ���G���Ԃ̏I��
		if (m_EnemyDamageCount > m_DamageEndTiming)
		{
			m_EnemyDamageCount = 0;

			m_pEnemyMesh->SetVisibility(true);
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ������----------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);

	// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	m_EnemyDamageCount = 0;

	CollisionOn();
	MeshOn();
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ��Ԃ̏�����
// �A�j���[�V�����؂�ւ� ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::ReSetState()
{
	m_EnemyState = ENEMY_STATE_IDLE;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �����蔻��̖�����------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::CollisionOff()
{
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_pEnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �����蔻��̗L����------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::CollisionOn()
{
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY))return;
	m_pCapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_pEnemyMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ���b�V���̕\��------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOn()
{
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY))return;
	SetActorHiddenInGame(false);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ���b�V���̔�\��------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOff()
{
	SetActorHiddenInGame(true);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �A�j���[�V�����؂�ւ�------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
