// ばけものチーム

#include "Enemy_Rote_Component.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UEnemy_Rote_Component::UEnemy_Rote_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_Rote_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	// プレイヤーの探索
	SerchPlayer();
}


// Called every frame
void UEnemy_Rote_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	// プレイヤーの探索
	SerchPlayer();

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UEnemy_Rote_Component::SerchPlayer()
{
	// 一回だけ
	if (m_pPlayerActor)return;

	// プレイヤーを探す
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	if (actors.Num() > 0)
	{
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			if (pActor->ActorHasTag("PlayerCharacter"))
			{
				m_pPlayerActor = pActor;
				break;
			}
		}
	}
}

void UEnemy_Rote_Component::EnemyRote()
{
	FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	FVector tempPlayerCharacterPosition = FVector::ZeroVector;
	if (m_pPlayerActor != NULL)
	{
		tempPlayerCharacterPosition = m_pPlayerActor->GetActorLocation();
	}


	if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
	{
		// 前向き
		tempEnemyRotation.Yaw = 90.f;
		m_StraightVector = false;
	}
	else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
	{
		// 後ろ向き
		tempEnemyRotation.Yaw = 270.f;
		m_StraightVector = true;
	}
	m_pEnemy->SetActorRotation(tempEnemyRotation);
}
void UEnemy_Rote_Component::TurnRote()
{
	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	if (tempEnemyRotation.Yaw >= 260.0f)
	{
		// 前向き
		tempEnemyRotation.Yaw = 90.f;
		m_StraightVector = false;
	}
	else 
	{
		// 後ろ向き
		tempEnemyRotation.Yaw = 270.f;
		m_StraightVector = true;
	}
	m_pEnemy->SetActorRotation(tempEnemyRotation);
}