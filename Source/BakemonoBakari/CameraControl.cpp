// 2021 05/01 ����I��
// �J�����̈ړ��������Ǘ�����
// 2021 05/10 ����I��
// �J�������ɓ����Ă��邩�𒲂ׂ�
// // 2021 05/10 ����I��
// �����l��ݒ�
// 2021 05/14 ����I��
// �J�������ɓ����Ă��邩�𒲂ׂ�X�V

#include "CameraControl.h"
#include "CameraSpline.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

#define ASPECT  1.77777f

ACameraControl::ACameraControl() :
	m_pActor(NULL),
	m_NowDistance(600.0f),
	m_NowSpeed(8.0f),
	m_Player(true),
	m_pPlayerActor(NULL),
	m_TargetPos(FVector::ZeroVector),
	m_SpeedHight(5.0f),
	m_SpeedWidth(3.0f),
	m_Distance(600.0f),
	m_LenghWidth(220.0f),
	m_LenghHight(100.0f),
	m_MaxSpeed(14.0f),
	m_Move(false),
	m_Right(true)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// ����������//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::BeginPlay()
{
	Super::BeginPlay();

	// ���x�Ƌ����̏�����
	m_NowSpeed = m_SpeedWidth;

	// �A�N�^�[����������
	Search();

	// �v���C���[�𒍖ڃA�N�^�[�ɂ���
	SetTargetPlayerActor();

	for (int n = 0; n < m_pSpline.Num(); ++n)
	{
		m_pSpline[n]->Start();
	}

	SearchSpline();

	// �J�����̏����ʒu��������
	SetActorLocation(FVector(m_TargetPos.X + m_Distance, m_TargetPos.Y, m_TargetPos.Z));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �A�N�^�[����������-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::Search()
{
	// �A�N�^�[��T������
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	for (int i = 0; i < actors.Num(); i++)
	{
		// �X�v���C����T��
		if (actors[i]->ActorHasTag("Spline"))
		{
			m_pSpline.Add(Cast<ACameraSpline>(actors[i]));
		}

		// �v���C���[���擾����
		else if (actors[i]->ActorHasTag("PlayerCharacter"))
		{
			m_pPlayerActor = actors[i];
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// �X�v���C������������-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::SearchSpline()
{
	if (m_pSpline.Num() == 0)return;

	// �}�b�v���̃X�v���C�����擾
	TArray<FVector> tempPoses;
	TArray<float> tempLengthes;

	FVector playerPos = m_pSpline[0]->GetPlayerPos();

	for (int n = 0; n < m_pSpline.Num(); ++n)
	{
		FVector tempPos = m_pSpline[n]->GetTargetPos();
		float y = playerPos.Y - tempPos.Y;
		float z = playerPos.Z - tempPos.Z;

		float length = FMath::Sqrt(y * y + z * z);

		tempPoses.Add(tempPos);
		tempLengthes.Add(length);
	}

	// �o�u���\�[�g�ň�ԋ߂��X�v���C���̍��W���擾
	for (int n = 0; n < tempPoses.Num(); ++n)
	{
		for (int i = tempPoses.Num() - 1; i > n; --i)
		{
			if (tempLengthes[i] < tempLengthes[i - 1])
			{
				float temp = tempLengthes[i];
				FVector tempPos = tempPoses[i];

				tempLengthes[i] = tempLengthes[i - 1];
				tempPoses[i] = tempPoses[i - 1];

				tempLengthes[i - 1] = temp;
				tempPoses[i - 1] = tempPos;
			}
		}
	}
	m_TargetPos = tempPoses[0];
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// �X�V����//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Player)
	{
		MovePlayerCamera();
	}
	else
	{
		MoveCamera();
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �J�����̃v���C���[�Ǐ]�̈ړ����s�� -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera()
{
	if (m_pSpline.Num() == 0)return;

	SearchSpline();

	// �J�����ƃv���C���[�̑��΋���
	float relativeDistance = m_TargetPos.Y - GetActorLocation().Y;

	// �J�����̈ړ����s�������肷��
	if (m_Move)
	{
		if (m_Right)
		{
			if (relativeDistance > m_AdjustmentPos.Y)
			{
				m_Move = false;
			}
		}
		else 
		{
			if (relativeDistance < -m_AdjustmentPos.Y)
			{
				m_Move = false;
			}
		}
	}
	else 
	{
		if (FMath::Abs(relativeDistance) > m_LenghWidth)
		{
			if ((relativeDistance > 0) && (m_pPlayerActor->GetActorRotation().Yaw >= 0))
			{
				m_FrontPos = FVector(0.0f, m_AdjustmentPos.Y, m_AdjustmentPos.Z);

				m_Right = false;

				m_Move = true;

			}
			else if ((relativeDistance < 0) && (m_pPlayerActor->GetActorRotation().Yaw <= -80.0f))
			{
				m_FrontPos = FVector(0.0f, -m_AdjustmentPos.Y, m_AdjustmentPos.Z);

				m_Right = true;

				m_Move = true;

			}
		}
	}

	// �ړ����s���ꍇ
	if (m_Move)
	{
		// �ړ���̖ڕW���W��ݒ�
		FVector targetPos = FVector(m_TargetPos.X + m_Distance, m_TargetPos.Y, m_TargetPos.Z);
		FVector move = FVector((targetPos.X - GetActorLocation().X) / m_NowSpeed, 0.0f, 0.0f);

		// ���ړ��������Z
		targetPos += m_FrontPos;
		float speed = (targetPos.Y - GetActorLocation().Y) / m_NowSpeed;

		if ((speed > 0.0f) && (speed > m_MaxSpeed))
		{
			speed = m_MaxSpeed;
		}
		else if ((speed < 0.0f) && (speed < -m_MaxSpeed))
		{
			speed = -m_MaxSpeed;
		}

		move += FVector(0.0f, speed, 0.0f);

		// �c�ړ��������Z
		move += FVector(0.0f, 0.0f, (targetPos.Z - GetActorLocation().Z) / m_SpeedHight);

		// �ړ�
		SetActorLocation(GetActorLocation() + move);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �J�����̃v���C���[�Ǐ]�ȊO�̈ړ����s�� -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MoveCamera()
{
	if (!m_pActor)return;
	// �ړ���̖ڕW���W��ݒ�
	FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_Distance, m_pActor->GetActorLocation().Y, m_pActor->GetActorLocation().Z);

	FVector move = (targetPos - GetActorLocation()) / m_NowSpeed;

	SetActorLocation(GetActorLocation() + move);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �_A�Ɠ_B�̋������v��-----------------------------------------------------------------------------------------------------------------------------------------------
float ACameraControl::Measurement(FVector _a, FVector _b)
{
	float y = FMath::Pow(_a.Y - _b.Y, 2);
	float z = FMath::Pow(_a.Z - _b.Z, 2);

	float lengh = FMath::Sqrt(y + z);
	return lengh;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// �����œn���ꂽ���W���J�����ɉf�邩�ǂ����𒲂ׂ�-----------------------------------------------------------------------------------------------------------------------------------------------
bool ACameraControl::CheckInCamera(FVector _pos, FVector _size)
{
	// �J�����Ƃ̋���
	float distance = _pos.X - GetActorLocation().X;

	float width = distance;
	float hight = distance / ASPECT;

	// �A�N�^�[�̃T�C�Y�ɕ�����
	width -= _size.X;
	hight -= _size.Y;

	FVector2D pos = FVector2D(GetActorLocation().Y - _pos.Y, GetActorLocation().Z - _pos.Z);

	// 2021 05 07 �������Ԉ���Ă������ߏC��
	if ((FMath::Abs(pos.X) > FMath::Abs(width)) || (FMath::Abs(pos.Y) > FMath::Abs(hight)))
	{
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
