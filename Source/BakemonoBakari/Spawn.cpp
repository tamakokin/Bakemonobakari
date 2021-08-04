// ばけものチーム


#include "Spawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawn::ASpawn():
m_Path("")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 指定されたパスのアクターを指定された個数生成---------------------------------------------------------------------------------------------------------------------
void ASpawn::SetUp(FString _path, int _index) 
{
	m_Path = _path;

	Tags.Add(FName(*m_Path));

	for (int n = 0; n < _index; ++n)
	{
		m_SpawnActor.Add(Spawn());
	}
}
//---------------------------------------------------------------------------------------------------------------------
// アクターを生成---------------------------------------------------------------------------------------------------------------------
AActor* ASpawn::Spawn()
{
	// アクター生成用
	TSubclassOf<class AActor> subClass;
	FVector location;
	FRotator rotation;
	AActor* actor = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("BB"));

	//パス指定したBPのアクタークラスを格納
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(m_Path)).LoadSynchronous();

	if (!subClass)return nullptr;

	 //位置と回転を設定
	location = GetActorLocation(); //+ m_pCapsuleComp->GetRightVector() * 10;
	rotation = FRotator(0.0f, 0.0f, 0.0f);

	// 設定した値を反映してスポーン
	actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

	actor->SetActorHiddenInGame(true);
	UE_LOG(LogTemp, Warning, TEXT("CC"));
	return actor;
}
//---------------------------------------------------------------------------------------------------------------------

// 使用されていないアクターを返す---------------------------------------------------------------------------------------------------------------------
AActor* ASpawn::ReturnActor() 
{
	for (int n = 0; n < m_SpawnActor.Num(); ++n)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), m_SpawnActor[n]->bHidden);

		if (m_SpawnActor[n]->bHidden) 
		{
			return m_SpawnActor[n];
		}
	}

	return Spawn();
}
//---------------------------------------------------------------------------------------------------------------------
