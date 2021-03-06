// ばけものチーム


#include "CameraSpline.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine.h"

// Sets default values
ACameraSpline::ACameraSpline():
	m_TargetPos(FVector::ZeroVector),
	m_PlayerPos(FVector::ZeroVector),
	m_pPlayer(NULL)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraSpline::Start()
{
	// プレイヤーアクターを探索する
	TSubclassOf<AActor>findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*>actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	// プレイヤーを取得する
	for (int i = 0; i < actors.Num(); i++)
	{
		if (actors[i]->ActorHasTag("PlayerCharacter"))
		{
			m_pPlayer = actors[i];
			break;
		}
	}

	SetPos();
}

// Called every frame
void ACameraSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPos();
}
void ACameraSpline::SetPos() 
{
	if (m_pPlayer)
	{
		m_PlayerPos = m_pPlayer->GetActorLocation();
		CheckPos();
	}
}
