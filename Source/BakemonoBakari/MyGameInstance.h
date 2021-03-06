// ばけものチーム
//更新履歴
// 2021/07/08 更新者：伴野　作成
// 2021/08/06 更新者：伴野　外部ファイルからスコアを読み込む

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
	SCORE_TYPE_NORMAL_ENEMY,
	SCORE_TYPE_BOSS_ENEMY,
	SCORE_TYPE_SECRET
};

UCLASS()
class BAKEMONOBAKARI_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UMyGameInstance();

public:
	// 直近で加算されたスコアをリセットする
	UFUNCTION(BlueprintCallable, Category = "Score")
		void ResetRecentAddedScore() { recentAddedScore = 0.f; }

	// スコアを加減算する
	UFUNCTION(BlueprintCallable, Category = "Score")
		void AddScore(int _p, SCORE_TYPE _s);

	// スコアをリセットする
	void ResetScore() { playerScore = 0; }

	// 外部ファイルからランキングを読み込む
	UFUNCTION(BlueprintCallable, Category = "Ranking")
		void ReadRankingScore();

	// 現在のスコアをランキングの値と比較して、記録を更新していればランキングを更新したうえで現在のスコアが該当する順位を返す
	UFUNCTION(BlueprintCallable, Category = "Ranking")
		int UpdateRanking();

	// 外部ファイルにランキングを保存する
	UFUNCTION(BlueprintCallable, Category = "Ranking")
		void WriteRankingScore();

public:
	// スコア
	UPROPERTY(BlueprintReadWrite, Category = "Score")
		int playerScore;

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

	// ランキング上位のスコア（TArrayで管理したかったが、Fstringからintに変換する段階でエラー落ちが発生したため断念）
	UPROPERTY(BlueprintReadWrite, Category = "Ranking")
		int rankingScore1;
	UPROPERTY(BlueprintReadWrite, Category = "Ranking")
		int rankingScore2;
	UPROPERTY(BlueprintReadWrite, Category = "Ranking")
		int rankingScore3;
	UPROPERTY(BlueprintReadWrite, Category = "Ranking")
		int rankingScore4;
	UPROPERTY(BlueprintReadWrite, Category = "Ranking")
		int rankingScore5;

};
