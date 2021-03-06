// ばけものチーム
// 更新履歴	：2021/08/27 松中海斗 BHiddenを使用している箇所をIsHiddenに変更

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
		Spawn();
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

	//パス指定したBPのアクタークラスを格納
	subClass = TSoftClassPtr<AActor>(FSoftObjectPath(m_Path)).LoadSynchronous();

	if (!subClass)return nullptr;

	 //位置と回転を設定
	location = GetActorLocation(); 
	rotation = FRotator(0.0f, 0.0f, 0.0f);

	// 設定した値を反映してスポーン
	actor = GetWorld()->SpawnActor<AActor>(subClass, location, rotation);

	actor->SetActorHiddenInGame(true);
	m_SpawnActor.Add(actor);
	return actor;
}
//---------------------------------------------------------------------------------------------------------------------

// 使用されていないアクターを返す---------------------------------------------------------------------------------------------------------------------
AActor* ASpawn::ReturnActor() 
{
	if (m_SpawnActor.Num() <= 0)return nullptr;

	for (int n = 0; n < m_SpawnActor.Num(); ++n)
	{
		if (m_SpawnActor[n]->IsHidden()) 
		{
			return m_SpawnActor[n];
		}
	}

	return Spawn();
}
//---------------------------------------------------------------------------------------------------------------------
