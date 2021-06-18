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
	m_NowDistance(800.0f),
	m_NowSpeed(0.0f),
	m_OldPos(FVector::ZeroVector),
	m_Player(true),
	m_MoveHight(true),
	m_MoveWidth(true),
	m_shockCount(0),
	m_shockTiming(0),
	m_shockMaxWidth(0.0f),
	m_shockMaxHeight(0.0f),
	m_shockStart(false),
	m_AdjustmentPos(FVector(0.0f, 150.0f, 100.0f)),
	m_TargetPos(FVector::ZeroVector),
	m_SpeedHight(5.0f),
	m_SpeedWidth(70.0f),
	m_Distance(800.0f),
	m_LenghWidth(400.0f),
	m_LenghHight(100.0f),
	m_MaxPos(FVector(2170.0f, 13480.0f, 550.0f)),
	m_MinPos(FVector(2170.0f, -13480.0f, -550.0f))
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
	m_NowDistance = m_Distance;

	// �v���C���[�A�N�^�[����������
	SearchPlayer();

	// �v���C���[�𒍖ڃA�N�^�[�ɂ���
	SetTargetPlayerActor();

	for (int n = 0; n < m_pSpline.Num(); ++n)
	{
		m_pSpline[n]->Start();
	}

	// �J�����̏����ʒu��������
	FVector targetPos = FVector(m_TargetPos.X + m_Distance, m_TargetPos.Y, m_TargetPos.Z);
	//SetActorLocation(m_pSpline->GetPlayerPos());

	//NoticePlayer();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �v���C���[�A�N�^�[����������-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::SearchPlayer()
{
	// �v���C���[�A�N�^�[��T������
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	// �v���C���[���擾����
	for (int i = 0; i < actors.Num(); i++)
	{
		if (actors[i]->ActorHasTag("Spline"))
		{
			m_pSpline.Add(Cast<ACameraSpline>(actors[i]));
		}
	}
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
	// �h����s��
	if (m_shockStart)
	{
		Shock();
	}

	// �J�������͈͂𒴂��Ȃ��悤�ɂ���
	//CheckInPos();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �J�����̃v���C���[�Ǐ]�̈ړ����s�� -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera()
{
	if (m_pSpline.Num() == 0)return;

	NoticePlayer();

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
				tempPoses[i] = tempPoses[i -  1];

				tempLengthes[i - 1] = temp;
				tempPoses[i - 1] = tempPos;
			}
		}
	}
	m_TargetPos = tempPoses[0];

	// �J�����̈ړ����s�������肷��
	if (FMath::Abs(m_TargetPos.Y - GetActorLocation().Y) > m_LenghWidth)
	{
		m_MoveWidth = true;
	}
	else
	{
		m_MoveWidth = false;
	}
	if (FMath::Abs(m_TargetPos.Z - GetActorLocation().Z) > m_LenghHight)
	{
		m_MoveHight = true;
	}
	else
	{
		m_MoveHight = false;
	}

	// �ړ���̖ڕW���W��ݒ�
	FVector targetPos = FVector(m_TargetPos.X + m_NowDistance, m_TargetPos.Y, m_TargetPos.Z);
	FVector move = FVector((targetPos.X - GetActorLocation().X) / m_NowSpeed, 0.0f, 0.0f);

	// ���ړ��������Z
	if (m_MoveWidth)
	{
		move += FVector(0.0f, (targetPos.Y - GetActorLocation().Y) / 2.0f, 0.0f);
	}
	else 
	{
		move += FVector(0.0f, (targetPos.Y - GetActorLocation().Y) / m_NowSpeed, 0.0f);

	}
	if (m_MoveHight) 
	{
		// �c�ړ��������Z
		move += FVector(0.0f, 0.0f, (targetPos.Z - GetActorLocation().Z) / 2.0f);
	}
	else 
	{
		// �c�ړ��������Z
		move += FVector(0.0f, 0.0f, (targetPos.Z - GetActorLocation().Z) / m_SpeedHight);
	}

	// �ړ�
	SetActorLocation(GetActorLocation() + move);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �J�����̃v���C���[�Ǐ]�ȊO�̈ړ����s�� -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MoveCamera()
{
	if (!m_pActor)return;
	// �ړ���̖ڕW���W��ݒ�
	FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_NowDistance, m_pActor->GetActorLocation().Y, m_pActor->GetActorLocation().Z);

	FVector move = (targetPos - GetActorLocation()) / m_NowSpeed;

	SetActorLocation(GetActorLocation() + move);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �v���C���[�̌����Ă�������ɍ��킹�ăJ�����̒��S���W��ύX//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::NoticePlayer()
{
	//if (m_pPlayerActor)
	//{
	//	// �v���C���[�E�������Ă���ꍇ�̓v���C���[���+X�̍��W�𒍖ڍ��W��
	//	if (m_pPlayerActor->GetActorRotation().Yaw >= 0)
	//	{
	//		m_TargetPos = m_pPlayerActor->GetActorLocation() + FVector(0.0f,m_AdjustmentPos.Y,m_AdjustmentPos.Z);
	//	}
	//	else
	//	{
	//		m_TargetPos = m_pPlayerActor->GetActorLocation() + FVector(0.0f, -m_AdjustmentPos.Y, m_AdjustmentPos.Z);
	//	}
	//}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// �J������h�炷-----------------------------------------------------------------------------------------------------------------------------------------------

void ACameraControl::Shock()
{
	if (m_shockCount % m_shockTiming == 0)
	{
		float width = FMath::RandRange(-m_shockMaxWidth, m_shockMaxWidth);
		float hight = FMath::RandRange(-m_shockMaxHeight, m_shockMaxHeight);

		SetActorLocation(FVector(0.0f, width, hight) + GetActorLocation());
	}
	// �J�E���g�����炷
	m_shockCount--;

	// �h�炷��Ԃ���߂�
	if (m_shockCount <= 0)
	{
		m_shockStart = false;
	}
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
// �J�������͈͊O�ɂ���ꍇ�͈͂ɖ߂�-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::CheckInPos()
{
	float width = GetActorLocation().Y;
	float hight = GetActorLocation().Z;

	auto max = [](float& _pos, float _value)
	{
		if (_pos > _value)
		{
			_pos = _value;
		}
	};
	auto min = [](float& _pos, float _value)
	{
		if (_pos < _value)
		{
			_pos = _value;
		}
	};
	max(width, m_MaxPos.Y);
	max(hight, m_MaxPos.Z);
	min(width, m_MinPos.Y);
	min(hight, m_MinPos.Z);

	SetActorLocation(FVector(GetActorLocation().X, width, hight));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------