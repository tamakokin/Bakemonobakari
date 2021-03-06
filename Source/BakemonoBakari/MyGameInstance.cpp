// ばけものチーム
//更新履歴
// 2021/07/08 更新者：伴野　作成
// 2021/08/06 更新者：伴野　外部ファイルからスコアを読み込む
// 2021/08/16 更新者：伴野　外部ファイルにスコアを書き出す
// 2021/08/25 更新者：松中　ヘッダーファイルの順番の修正


#include "MyGameInstance.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

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

// 外部ファイルからランキングを読み込む
void UMyGameInstance::ReadRankingScore()
{
	// 保存場所を設定
	FString filePath = FPaths::ProjectConfigDir() + "ScoreRanking.csv";

	// ファイルを読み込む
	FString csvFullData;
	if (FFileHelper::LoadFileToString(csvFullData, *filePath))
	{
		// 読み込んだデータを行ごとに分解
		TArray<FString> csvRowData;
		csvFullData.ParseIntoArrayLines(csvRowData);
		rankingScore1 = FCString::Atoi(*csvRowData[0]);
		rankingScore2 = FCString::Atoi(*csvRowData[1]);
		rankingScore3 = FCString::Atoi(*csvRowData[2]);
		rankingScore4 = FCString::Atoi(*csvRowData[3]);
		rankingScore5 = FCString::Atoi(*csvRowData[4]);

		/*
		for (int i = 0; i < csvRowData.Num(); ++i)
		{
			// FString型のデータをintに変換してデータとして取り込む
			rankingScore[i] = FCString::Atoi(*csvRowData[i]);
		}*/

		UE_LOG(LogTemp, Warning, TEXT("Ranking Loading Succes"));
	}
	// 読み込み失敗時
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Ranking Loading Failed"));
	}
}

// 現在のスコアをランキングの値と比較して、記録を更新していればランキングを更新したうえで現在のスコアが該当する順位を返す
int UMyGameInstance::UpdateRanking()
{
	if (playerScore > rankingScore1)
	{
		rankingScore5 = rankingScore4;
		rankingScore4 = rankingScore3;
		rankingScore3 = rankingScore2;
		rankingScore2 = rankingScore1;
		rankingScore1 = playerScore;

		return 1;
	}
	else if (playerScore > rankingScore2)
	{
		rankingScore5 = rankingScore4;
		rankingScore4 = rankingScore3;
		rankingScore3 = rankingScore2;
		rankingScore2 = playerScore;

		return 2;
	}
	else if (playerScore > rankingScore3)
	{
		rankingScore5 = rankingScore4;
		rankingScore4 = rankingScore3;
		rankingScore3 = playerScore;

		return 3;
	}
	else if (playerScore > rankingScore4)
	{
		rankingScore5 = rankingScore4;
		rankingScore4 = playerScore;

		return 4;
	}
	else if (playerScore > rankingScore5)
	{
		rankingScore5 = playerScore;

		return 5;
	}

	return 0;
}

// 外部ファイルにランキングを保存する
void UMyGameInstance::WriteRankingScore()
{
	// 保存場所を設定
	FString filePath = FPaths::ProjectConfigDir() + "ScoreRanking.csv";

	// 保存内容を設定
	FString csvFullData = "";
	csvFullData += FString::FString::FromInt(rankingScore1) + "\n";
	csvFullData += FString::FString::FromInt(rankingScore2) + "\n";
	csvFullData += FString::FString::FromInt(rankingScore3) + "\n";
	csvFullData += FString::FString::FromInt(rankingScore4) + "\n";
	csvFullData += FString::FString::FromInt(rankingScore5) + "\n";

	// 保存場所と保存内容を関数に渡して保存
	if (FFileHelper::SaveStringToFile(csvFullData, *filePath))
	{
	}
	// 読み込み失敗時
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Loading Failed"));
	}
}