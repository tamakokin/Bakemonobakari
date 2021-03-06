// ばけものチーム
// EnemyScore.cpp
// 概要		：敵を撃破時に数秒間出現するUIメッセージ
// 作成者	：伴野慧
// 作成日	：2021/9/6
// 更新履歴	：2021/9/10	出現の際に動きをつける（伴野）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "EnemyScore.generated.h"

class UProceduralMeshComponent;
class UMaterial;

UCLASS()
class BAKEMONOBAKARI_API AEnemyScore : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetScore(int _score);

private:
	void StartMove();				// 開始直後に動きをつける関数

public:
	UPROPERTY(EditAnywhere)
		FVector2D ScoreSize;		// 表示サイズ

	UPROPERTY(EditAnywhere)
		float DisplayDistance;		// プレイヤーの立っている位置から見てどれだけ手前に表示するか
	
	UPROPERTY(EditAnywhere)
		float DisplayTime;			// 表示時間

	UPROPERTY(EditAnywhere)
		float ScrollTime;			// 表示を開始してからサイズが最大になるまでの時間

	UPROPERTY(EditAnywhere)
		UProceduralMeshComponent* pMesh;		// メッシュオブジェクト

private:
	TArray<FVector> vertex;				// 頂点
	TArray<int32> vertexIndex;			// 頂点インデックス
	TArray<FVector2D> uv;				// テクスチャー座標
	TArray<FVector> normal;				// 法線
	TArray<FLinearColor> vertexColor;	// 頂点カラー群
	TArray<FProcMeshTangent> tangent;	// 接線群

	bool bDisplay;					// 表示しているか
	float DisplayTimer;				// 表示時間をカウント

};
