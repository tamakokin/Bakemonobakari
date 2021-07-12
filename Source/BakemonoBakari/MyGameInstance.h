// ばけものチーム
//更新履歴
// 2021/07/08 更新者：伴野　作成

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// どういう種類のスコア獲得なのか
UENUM(BlueprintType)
enum class SCORE_TYPE :uint8
{
	SCORE_TYPE_NONE = 0,
	SCORE_TYPE_TIME,
	SCORE_TYPE_NORMAL_ENEMY
};

UCLASS()
class BAKEMONOBAKARI_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UMyGameInstance();

public:

	// スコアを取得する
	UFUNCTION(BlueprintCallable, Category = "Score")
		int GetScore() { return playerScore; }

	// 直近で加算されたスコアを取得する
	UFUNCTION(BlueprintCallable, Category = "Score")
		int GetRecentAddedScore() { return recentAddedScore; }
	UFUNCTION(BlueprintCallable, Category = "Score")
		SCORE_TYPE GetRecentAddedScoreType() { return recentAddedScoreType; }

	// スコアを加減算する
	UFUNCTION(BlueprintCallable, Category = "Score")
		void AddScore(int _p, SCORE_TYPE _s);

	// スコアをリセットする
	void ResetScore() { playerScore = 0; }

	UFUNCTION(BlueprintCallable, Category = "Score")
		void ResetRecentAddedScore() { recentAddedScore = 0.f; }

private:
	// スコア
	int playerScore;

public:
	// 直近で加算されたスコア
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int recentAddedScore;

	// 直近でスコアが加算された理由
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		SCORE_TYPE recentAddedScoreType;

	// 直近で加算されたスコアをリセットするためのタイマー
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		float recentAddedScoreTimer;

	// プレーが開始してからの経過時間
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		float playingTimer;
};
