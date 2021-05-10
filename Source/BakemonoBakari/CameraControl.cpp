// 2021 05/01 大金巧侑
// カメラの移動処理を管理する

#include "CameraControl.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

ACameraControl::ACameraControl() :
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

// 初期化処理//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::BeginPlay()
{
    Super::BeginPlay();

    // 速度と距離の初期化
    m_NowSpeed = m_SpeedWidth;
    m_NowDistance = m_Distance;

    // プレイヤーアクターを検索する
    SearchPlayer();

    // プレイヤーを注目アクターにする
    SetTargetPlayerActor();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// プレイヤーアクターを検索する-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::SearchPlayer()
{
    // プレイヤーアクターを探索する
    TSubclassOf<AActor>findClass;
    findClass = AActor::StaticClass();
    TArray<AActor*>actors;
    UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

    // プレイヤーを取得する
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
// 更新処理//-----------------------------------------------------------------------------------------------------------------------------------------------
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
    // 揺れを行う
    if (m_shockStart)
    {
        Shock();
    }

    // カメラが範囲を超えないようにする
    CheckInPos();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// カメラのプレイヤー追従の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MovePlayerCamera()
{
    NoticePlayer();
    if (Measurement(m_TargetPos, GetActorLocation()) < m_Lengh)return;

    if (m_pActor)
    {
        // 移動後の目標座標を設定
        FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_NowDistance, m_TargetPos.Y, m_TargetPos.Z);
        FVector move = FVector((targetPos.X - GetActorLocation().X) / m_NowSpeed, (targetPos.Y - GetActorLocation().Y) / m_NowSpeed, (targetPos.Z - GetActorLocation().Z) / m_SpeedHight);

        SetActorLocation(GetActorLocation() + move);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// カメラのプレイヤー追従以外の移動を行う -----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::MoveCamera()
{
    if (!m_pActor)return;
    // 移動後の目標座標を設定
    FVector targetPos = FVector(m_pActor->GetActorLocation().X + m_NowDistance, m_pActor->GetActorLocation().Y, m_pActor->GetActorLocation().Z);

    FVector move = (targetPos - GetActorLocation()) / m_NowSpeed;

    SetActorLocation(GetActorLocation() + move);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// プレイヤーの向いている方向に合わせてカメラの中心座標を変更//-----------------------------------------------------------------------------------------------------------------------------------------------
void ACameraControl::NoticePlayer()
{
    if (m_pPlayerActor)
    {
        // プレイヤー右を向いている場合はプレイヤーより+Xの座標を注目座標に
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
// カメラを揺らす-----------------------------------------------------------------------------------------------------------------------------------------------

void ACameraControl::Shock()
{
    if (m_shockCount % m_shockTiming == 0)
    {
        float width = FMath::RandRange(-m_shockMaxWidth, m_shockMaxWidth);
        float hight = FMath::RandRange(-m_shockMaxHeight, m_shockMaxHeight);

        SetActorLocation(FVector(0.0f, width, hight) + GetActorLocation());
    }
    // カウントを減らす
    m_shockCount--;

    // 揺らす状態をやめる
    if (m_shockCount <= 0)
    {
        m_shockStart = false;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

// 点Aと点Bの距離を計る-----------------------------------------------------------------------------------------------------------------------------------------------
float ACameraControl::Measurement(FVector _a, FVector _b)
{
    float y = FMath::Pow(_a.Y - _b.Y, 2);
    float z = FMath::Pow(_a.Z - _b.Z, 2);

    float lengh = FMath::Sqrt(y + z);
    return lengh;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// 引数で渡された座標がカメラに映るかどうかを調べる-----------------------------------------------------------------------------------------------------------------------------------------------
bool ACameraControl::CheckInCamera(FVector _pos)
{
    // カメラとの距離
    float distance = _pos.X - GetActorLocation().X;

    float width = distance;
    float hight = distance / 1.77777f;

    FVector2D pos = FVector2D(GetActorLocation().Y - _pos.Y, GetActorLocation().Z - _pos.Z);
   
    // 2021 05 07 条件が間違っていたため修正
    if ((FMath::Abs(pos.X) > FMath::Abs(width)) || (FMath::Abs(pos.Y) > FMath::Abs(hight)))
    {
        return false;
    }
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
// カメラが範囲外にいる場合範囲に戻す-----------------------------------------------------------------------------------------------------------------------------------------------
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