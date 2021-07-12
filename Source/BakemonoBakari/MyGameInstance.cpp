// ばけものチーム
//更新履歴
// 2021/07/08 更新者：伴野　作成


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
	: playerScore(0.f)
	, recentAddedScore(0.f)
{
}

void UMyGameInstance::AddScore(int _p, SCORE_TYPE _s)
{
	playerScore += _p;
	recentAddedScore = _p;
	recentAddedScoreType = _s;

	// タイマーをリセット
	recentAddedScoreTimer = 0.f;
}