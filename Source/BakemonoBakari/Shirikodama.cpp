// Shirikodama.cpp
// �T�v		�F�K�q�ʂ̋���
// �쐬��	�F�����C�l
//			�F����I��
// �쐬��	�F2021/07/09
// �X�V����	�F

#include "Shirikodama.h"

AShirikodama::AShirikodama() :
	m_MoveSpeed(10.0f),
	m_RisingPower(12.0f),
	m_RisingPowerNow(0.0f),
	m_Gravity(0.0f),
	m_Count(0),
	m_pMesh(NULL)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShirikodama::BeginPlay()
{
	Super::BeginPlay();

	// ���b�V����T��
	m_pMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (m_pMesh)
	{
		m_pMesh->OnComponentBeginOverlap.AddDynamic(this, &AShirikodama::BeginHit);
	}
}

void AShirikodama::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
}

void AShirikodama::MoveStart(float _speed, float _risingPower)
{
	m_MoveSpeed = _speed;
	m_RisingPower = _risingPower;
	SetActorHiddenInGame(false);
	m_pMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// �ړ�����------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::Move()
{
	if (!bHidden)
	{
		m_Count++;

		// �ړ���
		FVector move = FVector::ZeroVector;

		// ���ړ�
		move.Y += m_MoveSpeed;

		if (!m_IsPlayerAttack)
		{
			// �c�ړ�
			m_Gravity = 0.0098f * m_Count * m_Count;
			m_RisingPowerNow = m_RisingPower - m_Gravity;
			move.Z += m_RisingPowerNow;
		}
		SetActorLocation(GetActorLocation() + move);

		if (m_IsSquashed)
		{
			m_Count = 0.0f;

			m_IsSquashed = false;
		}

		// ���̍��W�𒴂���Ə�����
		if (GetActorLocation().Y > m_LimitPosY)
		{
			// �폜����
			Des();
		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------
// �폜����------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::Des()
{
	m_pMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	m_Count = 0;
	m_IsSquashed = false;
}
//------------------------------------------------------------------------------------------------------------------------------------------

// �I�[�o�[���b�v�֐�------------------------------------------------------------------------------------------------------------------------------------------
void AShirikodama::BeginHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �A�N�^�[�̃|�C���^�[���ʂ�łȂ����
	if (OtherActor != NULL)
	{
		// �n�ʂɓ����������o�E���h����
		if (OtherActor->ActorHasTag("Ground"))
		{
			if (m_RisingPowerNow < 0.0f)
			{
				m_IsSquashed = true;
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------