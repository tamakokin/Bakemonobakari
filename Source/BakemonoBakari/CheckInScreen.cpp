// ばけものチーム
// 2021 05/14 大金巧侑
// カメラ内に入っているかをCameraControlに聞くコンポーネント


#include "CheckInScreen.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"
#include "CameraControl.h"

// Sets default values for this component's properties
UCheckInScreen::UCheckInScreen():
	m_Size(FVector(400.0f,400.0f,400.0f)),
	m_pCamera(NULL)
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCheckInScreen::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーアクターを探索する
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	// プレイヤーを取得する
	for (int i = 0; i < actors.Num(); i++)
	{
		if(Cast<ACameraControl>(actors[i]) != NULL)
		m_pCamera = Cast<ACameraControl>(actors[i]);
	}
}

// Called every frame
void UCheckInScreen::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
bool UCheckInScreen::Check(FVector _pos) 
{
	return m_pCamera->CheckInCamera(_pos, m_Size);
}

