// 2021 05/01 ����I��
// �J�����̈ړ��������Ǘ�����

#include "CameraControl.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

ACameraControl::ACameraControl():
    m_pActor(NULL),
    m_NowDistance(0.0f),
    m_NowSpeed(0.0f),
    m_TargetPos(FVector::ZeroVector),
    m_OldPos(FVector::ZeroVector),
    m_Player(true),
    m_shockCount(0),
    m_shockTiming(0),
    m_shockMaxWidth(0.0f),
    m_shockMaxHeight(0.0f),
    m_shockStart(false),
    m_pPlayerActor(NULL),
    m_AdjustmentPos(FVector::ZeroVector),
    m_SpeedHight(0.0f),
    m_SpeedWidth(0.0f),
    m_Distance(0.0f),
    m_Lengh(0.0f),
    m_MaxPos(FVector::ZeroVector),
    m_MinPos(FVector::ZeroVector)
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
        if (actors[i]->ActorHasTag("Player"))
        {
            m_pPlayerActor = actors[i];
            break;
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
    CheckInPos();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// �J�����̃v���C���[�Ǐ]�̈ړ����s�� -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera()
{
    NoticePlayer();
    if (Measurement(m_TargetPos, GetActorLocation()) < m_Lengh)return;

    if (m_pActor) 
    {
        // �ړ���̖ڕW���W��ݒ�
        FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_NowDistance, m_TargetPos.Y, m_TargetPos.Z);
        FVector move = FVector((targetPos.X - GetActorLocation().X) / m_NowSpeed, (targetPos.Y - GetActorLocation().Y) / m_NowSpeed, (targetPos.Z - GetActorLocation().Z) / m_SpeedHight);

        SetActorLocation(GetActorLocation() + move);
    }
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
    if (m_pPlayerActor)
    {
        // �v���C���[�E�������Ă���ꍇ�̓v���C���[���+X�̍��W�𒍖ڍ��W��
        if (m_pPlayerActor->GetActorRotation().Yaw >= 0)
        {
            m_TargetPos = m_pPlayerActor->GetActorLocation() + m_AdjustmentPos;
        }
        else
        {
            m_TargetPos = m_pPlayerActor->GetActorLocation() - m_AdjustmentPos;
        }
    }
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
bool ACameraControl::CheckInCamera(FVector _pos)
{
   // �J�����Ƃ̋���
    float distance = _pos.X - m_Distance;

    float width =  GetActorLocation().Y + distance;
    float hight = (GetActorLocation().Z + distance) / 1.77777f;

    if((FMath::Abs(_pos.Y) >width)||(FMath::Abs(_pos.Z) > hight))
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

    SetActorLocation(FVector( GetActorLocation().X,width,hight));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
