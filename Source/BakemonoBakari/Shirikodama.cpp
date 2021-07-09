// Shirikodama.cpp
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：


#include "Shirikodama.h"

AShirikodama::AShirikodama()
	:v(1.0f), waveLength(1.0f), amplitude(1.0f), time(0.0f)
{
	PrimaryActorTick.bCanEverTick = true;

}

void AShirikodama::BeginPlay()
{
	Super::BeginPlay();
}

void AShirikodama::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	time += DeltaTime;
	AddActorLocalOffset(FVector(0.0f, v * time, sinf(waveLength * time) * amplitude));
}

