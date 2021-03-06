// Stage01_SmallBoss.h
// 概要		：ステージ１の大きいボスの挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：


#include "Stage01_SmallBoss.h"
AStage01_SmallBoss::AStage01_SmallBoss()
	:m_v0(0.0f), m_a(1.0f), m_time(0.0f)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;
}

// Y方向の速度の計算
void AStage01_SmallBoss::MovementY(float _DeltaTime)
{
	float s = m_v0 + m_a * m_time;
	m_time += _DeltaTime;
	AddActorLocalOffset(FVector(0.0f, s, 0.0f));
}

// 毎フレームの処理
void AStage01_SmallBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Y方向の速度の計算
	MovementY(DeltaTime);
}