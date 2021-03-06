// ばけものチーム
#include "Cucumber.h"
#include "CheckInScreen.h"
// Sets default values
ACucumber::ACucumber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACucumber::BeginPlay()
{
	Super::BeginPlay();

	m_pCheckInScreen = Cast<UCheckInScreen>(GetComponentByClass(UCheckInScreen::StaticClass()));

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	m_pBox = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	m_pBox->OnComponentBeginOverlap.AddDynamic(this, &ACucumber::OnOverlapBegin);
}

// Called every frame
void ACucumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_pCheckInScreen)return;

	if(!IsHidden()) 
	{
		if (!m_pCheckInScreen->Check(GetActorLocation()))
		{
			Des();
		}

		// きゅうりの移動処理
		if (m_IsMove)
		{
			// 移動する向きに合わせて移動させる
			if (m_IsRight)
			{
				SetActorLocation(GetActorLocation() + FVector(0.0f, -m_Speed, 0.0f));

				// きゅうりを回転させる
				SetActorRotation(FRotator(0.0f, 0.0f, GetActorRotation().Roll - m_Speed));
			}
			else
			{
				SetActorLocation(GetActorLocation() + FVector(0.0f, m_Speed, 0.0f));

				// きゅうりを回転させる
				SetActorRotation(FRotator(0.0f, 0.0f, GetActorRotation().Roll + m_Speed));
			}
		}

		// 時間をカウントし生存期間を超えた場合に消す
		m_LifeCount++;
		if (m_LifeCount > m_LifeTime)
		{
			// 消す
			Des();
		}
	}
}
// 移動設定を行う---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ACucumber::MoveStart(float _speed,bool _right)
{
	m_Speed = _speed;
	m_IsRight = _right;
	m_IsMove = true;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	m_pBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 削除を行う---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ACucumber::Des() 
{
	// エフェクトを出す
	DesEfect();

	m_Speed = 0.0f;
	m_LifeCount = 0;

	SetActorHiddenInGame(true);
	m_pBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// オーバーラップ関数
UFUNCTION() void ACucumber::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// アクターのポインターがぬるでなければ
	if (OtherActor != NULL)
	{
		// 接触したアクターのタグが地面であれば
		if ((OtherComp->ComponentHasTag("Ground"))||(OtherActor->ActorHasTag("Ground")))
		{
			// 消す
			Des();
		}
	}
}