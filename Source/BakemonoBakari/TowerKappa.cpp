// TowerKappa.cpp
// 概要		：タワーかっぱの挙動
// 作成者	：松中海斗
// 作成日	：2021/07/09
// 更新履歴	：2021/07/15 更新者：陳　タワーを作る関数と必要の情報変数を作る
//　　　　　：2021/07/19 更新者：陳　タワーを作る関数と必要の情報変数を更新


#include "TowerKappa.h"

ATowerKappa::ATowerKappa()
	: _IsTower(false)
	, _IsDamage(false)
	, _IsInvincible(false)
{
	// メッシュを探す
	//m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
}


void ATowerKappa::AttackPhase1()
{
	//タワーの個数、フェースを設定
	_TowerHP = 3;
	_BossPhase = 0;		//0はフェース「一」
	//BuildTower(_BossPhase);

	CreateEvent();
}

void ATowerKappa::AttackPhase2()
{
	_TowerHP = 5;
	_BossPhase = 1;		//1はフェース「二」
	//BuildTower(_BossPhase);

	CreateEvent();
}

void ATowerKappa::AttackPhase3()
{
	_TowerHP = 7;
	_BossPhase = 2;		//はフェース「三」
	//BuildTower(_BossPhase);

	CreateEvent();
}

void ATowerKappa::BeginPlay()
{
	Super::BeginPlay();
}

void ATowerKappa::Tick(float DeltaTime)
{
	if (_IsDamage == true && _IsInvincible == false)
	{
		DamageEvent();
		_IsInvincible = true;
	}

	if (_TowerHP == 0 && _IsDamage == true)
	{
		_IsDamage = false;
		BreakTower();
	}
}

// タワーを作る関数　(陳7/15)
void ATowerKappa::BuildTower(int _phase)
{
	// ボスのフェースによって、作るタワーの個数を変更する
	switch (_phase)
	{
	case 1:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("3個"));

		AttackPhase1();

		break;

	case 2:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("5個"));

		AttackPhase2();

		break;

	case 3:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("7個"));

		AttackPhase3();

		break;
	}

	//m_pCapsuleComp->RelativeLocation.Z = 225.f;
}

// タワーが攻撃を受ける関数　(陳7/15)
void ATowerKappa::BreakTower()
{
	DestroyEvent();
}
