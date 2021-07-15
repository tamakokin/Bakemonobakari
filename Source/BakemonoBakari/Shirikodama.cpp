// Shirikodama.cpp
// 概要		：尻子玉の挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：


#include "Shirikodama.h"

AShirikodama::AShirikodama()
	:m_beginPosition(), m_time(), m_waveLength(1.0f), m_amplitude(1.0f)
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

	m_time += DeltaTime;
	AddActorLocalOffset(FVector(0.0f, m_v * m_time, sinf(m_waveLength * m_time) * m_amplitude));
}

