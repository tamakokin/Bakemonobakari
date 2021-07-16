// ばけものチーム
// 2021/06/14 大金巧侑
// エネミーのリスポーン
#include "ReStart.h"
#include "Kismet/GameplayStatics.h"
#include"EnemyActor.h"

// Sets default values
AReStart::AReStart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AReStart::BeginPlay()
{
	Super::BeginPlay();

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
			if (pActor->ActorHasTag("Enemy"))
			{
				m_pEnemyActors.Add(Cast<AEnemyActor>(pActor));
			}
		}
	}
}

void AReStart::ReStartPosition()
{
	for (int n = 0; n < m_pEnemyActors.Num(); ++n)
	{
		m_pEnemyActors[n]->ReStartPosition();
	}
}

// Called every frame
void AReStart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



