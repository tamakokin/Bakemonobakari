// AEnemyActor.cpp
// �T�v		�F�G�̍s���v���O����(�v���g�^�C�v
// �쐬��	�F��c�I��
// �쐬��	�F2021/5/7
// �X�V����	�F2021/5/10 �v���O�����̃R�����g��ǋL
//			�F2021/5/17 �W�����v����G�̍s���v���O������ǉ�
//			�F2021/5/23 ���Ŏ��̉���ǉ��i����j
//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
//           
#include "EnemyActor.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "CheckInScreen.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	//			�F2021/5/29 ��ʊO�ɂ���ꍇ�͓����Ȃ��悤�ɂ���i����j
	if (!m_pCheckInScreen->Check(GetActorLocation()))
	{
		m_IsInScreen = false;
		// �����蔻��̖�����
		CollisionOff();
		// ���b�V���̖�����
		MeshOff();
		return;
	}
	else if ((m_EnemyState != ENEMY_STATE_DESTROY)&&(m_EnemyDamageCount <= 0))
	{
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
		}
	}
	m_pOverlappedActor = NULL;
}

// �G�l�~�[�X�e�[�^�X�R���g���[��
void AEnemyActor::EnemyStatusControl(float _deltaTime)
{
	ChangeAnim();
}

// �_���[�W�֐�
void AEnemyActor::EnemyDamage()
{
	// �_���[�W��Ԃ�������
	if ((m_EnemyState == ENEMY_STATE_DAMAGE) || (m_EnemyState == ENEMY_STATE_DESTROY)) return;

	// ���G��ԂȂ�
	if (m_EnemyDamageCount > 0)return;

	// ���G��Ԃɂ���
	m_EnemyDamageCount = 1;

	// �����蔻��̖�����
	CollisionOff();

	// �q�b�g�G�t�F�N�g����
	// �A�N�^�[�����p
	FString BulletPath;
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor;
	// BP������p�X���w��

	BulletPath = "/Game/Effects/Hit_EffectBP.Hit_EffectBP_C";
	// �p�X�w�肵��BP�̃A�N�^�[�N���X���i�[
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(BulletPath)).LoadSynchronous();

	if (subClass)
	{
		// �ʒu�Ɖ�]��ݒ�
		location = GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("%s"), *m_pEnemyMesh->Bounds.BoxExtent.ToString());

		// �G�l�~�[�ɂ߂荞�܂Ȃ���ɑO�ɏo��
		location.X += 100.0f;
		rotation = FRotator(0.0f, 0.0f, 0.0f);

		// �ݒ肵���l�𔽉f���ăX�|�[��
		actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);
	}

	// HP�����炷�B�����̓v���C���[�̍U���l���Q�Ƃ���悤�ɂ��Ă���������
	--m_EnemyHP;
	//m_EnemyState = ENEMY_STATE_DAMAGE;
	ChangeAnim();

	if (m_EnemyHP <= 0)
	{
		//------------------------------------------------
		//���S�G�t�F�N�g����������
		//------------------------------------------------
		m_EnemyState = ENEMY_STATE_DESTROY;
		ChangeAnim();

		// �X�R�A�����Z����
		Cast<UMyGameInstance>(GetGameInstance())->AddScore(m_score, SCORE_TYPE::SCORE_TYPE_NORMAL_ENEMY);

		// �U�������o��
		if (m_crashSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, m_crashSound, GetActorLocation());
		}
	}
}

// ���G���̃}�e���A���̕ω�------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���G���ԊJ�n
void AEnemyActor::EnemyFlashing()
{
	int redCount = 5.0f;

	m_EnemyDamageCount++;

	if ((m_EnemyDamageCount % (redCount * 2)) == 0)
	{
		// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	}
	else if ((m_EnemyDamageCount % redCount) == 0)
	{
		// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.3f,0.0f,0.0f));
	}

	// ���G���Ԃ̏I��
	if (m_EnemyDamageCount >= 50) 
	{
		m_EnemyDamageCount = 0;
		// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
		m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));

		// �R���C�_�[�𕜋A
		CollisionOn();
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ������
void AEnemyActor::ReStartPosition()
{
	m_EnemyHP = m_EnemyHPMax;
	m_EnemyState = ENEMY_STATE_IDLE;

	SetActorLocation(m_initEnemyPosition);
	SetActorRotation(m_StartRote);

	// �}�e���A�����́uOpacity�v�p�����[�^�ɐ��l��ݒ肷��
	m_pEnemyMesh->SetVectorParameterValueOnMaterials(TEXT("Flashing"), FVector(0.0f, 0.0f, 0.0f));
	m_EnemyDamageCount = 0;
}

// ��Ԃ̏�����
// �A�j���[�V�����؂�ւ�
void AEnemyActor::ReSetState()
{
	m_EnemyState = ENEMY_STATE_IDLE;
}

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
	m_pEnemyMesh->SetVisibility(true);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ���b�V���̔�\��------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AEnemyActor::MeshOff()
{
	m_pEnemyMesh->SetVisibility(false);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// �A�j���[�V�����؂�ւ�
void AEnemyActor::ChangeAnim()
{
	// ���݂̃A�N�^�[�̏�Ԃ��r
	switch (m_EnemyState)
	{
	case AEnemyActor::ENEMY_STATE_NONE:
		break;

	case AEnemyActor::ENEMY_STATE_MOVE:
		UE_LOG(LogTemp, Warning, TEXT("Anime1"));
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
		UE_LOG(LogTemp, Warning, TEXT("Anime"));
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
