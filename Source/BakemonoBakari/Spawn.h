// ばけものチーム
// スポーンする必要のあるアクターを開始時に生成し使いまわす機能
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawn.generated.h"

UCLASS()
class BAKEMONOBAKARI_API ASpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 開始時に生成する
	void SetUp(FString _path, int index);

	// アクターの生成
	AActor* Spawn();

	// 使われていないアクターをかえす
	AActor* ReturnActor();

public:
	// 開始時にスポーンしたいアクターのポインタを保持
	TArray<AActor*> m_SpawnActor;

	// 生成するアクターのパス
	FString m_Path;
};
