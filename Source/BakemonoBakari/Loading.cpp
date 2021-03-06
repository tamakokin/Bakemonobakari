// ばけものチーム
// 更新履歴
// 2021/08/27 更新者：山田　作成

#include "Loading.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"

#define MAXTIME 2.f		

// Sets default values
ALoading::ALoading()
	: m_angle(0.f)
	, m_TimeCnt(0.f)
	, m_CheckCnt(0)
	, m_Completion(false)
	, m_Reset(false)
	, m_CheckBottan(false)
	, m_Fps(0.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//メッシュコンポーネント生成
	m_pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("pMesh"));
	if (m_pMesh != NULL)
	{
		RootComponent = m_pMesh;                //メッシュをルートにする
	}

}

// Called when the game starts or when spawned
void ALoading::BeginPlay()
{
	Super::BeginPlay();
}

// Called to bind functionality to input
void ALoading::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LoadingAction", IE_Pressed, this, &ALoading::ActionBottan);
}

// Called every frame
void ALoading::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_CheckBottan)
	{
		m_Fps += DeltaTime;
		
		if (m_Fps >= 1.0f)
		{
			Reset(true);
		}
	}
	else
	{
		// チェックポイントを超えたら
		if (m_CheckCnt > CHECKPOINT)
		{
			Reset(true);
			m_CheckCnt = 0;
		}
		else
		{
			// 移動処理
			UpdateMove(DeltaTime);

			// 一度だけ
			if (m_Reset)
			{
				m_CheckCnt++;
				m_Reset = false;
			}
		}
	}
}

// 移動処理
void ALoading::UpdateMove(float _deltaTime)
{
	// ぐるぐる
	//FVector NowLotation = this->GetActorLocation();
	//FRotator NowRotation = this->GetActorRotation();

	//float angle = atan2(NowLotation.Y + 20.f, NowLotation.X + 20.f) - atan2(NowLotation.Y, NowLotation.X);
	//float angleDeg = FMath::RadiansToDegrees(angle);

	//m_pMesh->SetRelativeRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw + angleDeg, NowRotation.Roll));
	
	FVector NowLocation = this->GetActorLocation();
	float NewLocationY = FMath::Abs(NowLocation.Y - m_EnemyLocation[m_CheckCnt].Y);

	// 回転が終了したら
	if (m_Completion)
	{
		if (m_TimeCnt <= NewLocationY /*+ 2.f && NowLocation.Y <= m_EnemyLocation[m_CheckCnt].Y - 2.f*/)
		{
			// 位置の計算
			m_TimeCnt += _deltaTime * 1.5f;
			if (NowLocation.Y <= m_EnemyLocation[m_CheckCnt].Y)
			{
				m_angle = m_TimeCnt;
				//UE_LOG(LogTemp, Display, TEXT("0"));
			}
			else
			{
				m_angle = -m_TimeCnt;
				//UE_LOG(LogTemp, Display, TEXT("1"));
			}
			//UE_LOG(LogTemp, Display, TEXT("false"));

		}
		else
		{
			Reset(false);
			m_Completion = false;
		}
		//UE_LOG(LogTemp, Display, TEXT("false"));
		// 位置を設定
		SetActorLocation(FVector(NowLocation.X, NowLocation.Y + m_angle, NowLocation.Z));
	}
	else
	{
		// 方向替え
		UpdateDirection(_deltaTime);
		//UE_LOG(LogTemp, Display, TEXT("true"));

	}
}

// 方向取得
void ALoading::UpdateDirection(float _deltaTime)
{
	FRotator NowRotation = this->GetActorRotation();
	float NewRotaton = FMath::Abs(NowRotation.Yaw - m_EnemyRotation[m_CheckCnt]);

	// 方向への取得
	if (m_TimeCnt <= NewRotaton /*+ 2.f && NowRotation.Yaw <= m_EnemyRotation[m_CheckCnt] - 2.f*/)
	{
		m_TimeCnt += _deltaTime * 1.5f;
		if (NowRotation.Yaw <= m_EnemyRotation[m_CheckCnt])
		{
			m_angle = m_TimeCnt;
			UE_LOG(LogTemp, Display, TEXT("0"));

		}
		else
		{
			m_angle = -m_TimeCnt;
			UE_LOG(LogTemp, Display, TEXT("1"));

		}
		// 方向を設定
		m_pMesh->SetRelativeRotation(FRotator(NowRotation.Pitch, NowRotation.Yaw + m_angle, NowRotation.Roll));
	}
	// 次の方向を計算
	else
	{
		//UE_LOG(LogTemp, Display, TEXT("comp"));

		//m_angle = m_EnemyRotation[m_CheckCnt];
		m_Completion = true;
		m_Reset = true;
		Reset(false);
	}
}

//// 計算取得
//float ALoading::Calculation(float _Now)
//{
//
//}

// アニメーション初期化
void ALoading::Reset(bool _anim)
{
	if (_anim)
	{
		m_EnemyAnim[0] = 0;
		m_EnemyAnim[1] = 0;
		m_EnemyAnim[2] = 0;
		m_Fps = 0.0f;
		m_CheckBottan = false;
	}
	else
	{
		m_angle = 0.f;
		m_TimeCnt = 0.f;
	}
}

// アクションボタン
void ALoading::ActionBottan()
{
	m_EnemyAnim[1] = 1;
	m_CheckBottan = true;
	UE_LOG(LogTemp, Display, TEXT("ActionBottan"));

}