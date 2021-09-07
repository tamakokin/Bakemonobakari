// ばけものチーム
// EnemyScore.cpp
// 概要		：敵を撃破時に数秒間出現するUIメッセージ
// 作成者	：伴野慧
// 作成日	：2021/9/6


#include "EnemyScore.h"
#include "ProceduralMeshComponent.h"

class UMaterial;

// Sets default values
AEnemyScore::AEnemyScore()
	: bDisplay(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// メッシュを作成しRootComponentにアタッチする
	pMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("generated-mesh"));
	pMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyScore::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyScore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 消えるまでの時間を計測
	if (bDisplay)
	{
		DisplayTimer += DeltaTime;

		if (DisplayTimer > DisplayTime)
		{
			Destroy();
		}
	}
}

void AEnemyScore::SetScore(int _score)
{

	// スコアの桁数をカウント
	int ScoreDigits = 1;
	while ( _score > (int)(FMath::Pow(10.0f, (float)ScoreDigits)) )
	{
		++ScoreDigits;
	}

	// メッシュ作成に必要な値を準備
	float MeshQuantity = (float)(ScoreDigits + 1);				// 四角いメッシュの枚数（「+100」のように表示するためメッシュは桁数+1枚必要）
	float StartPoint = ScoreSize.X * -0.5f * MeshQuantity;		// メッシュの左端の位置
	
	for (int i = 0; i <= ScoreDigits; ++i)
	{
		vertex.Empty();
		vertexIndex.Empty();
		uv.Empty();

		// 頂点座標
		vertex.Add(FVector(DisplayDistance * -1.f, ScoreSize.X * (float)i + StartPoint, ScoreSize.Y));			// 左上
		vertex.Add(FVector(DisplayDistance * -1.f, ScoreSize.X * (float)i + StartPoint, 0.f));					// 左下
		vertex.Add(FVector(DisplayDistance * -1.f, ScoreSize.X * (float)(i+1) + StartPoint, ScoreSize.Y));		// 右上
		vertex.Add(FVector(DisplayDistance * -1.f, ScoreSize.X * (float)(i+1) + StartPoint, 0.f));				// 右下
		
		// 頂点インデックス
		vertexIndex.Add(0);	// 左上
		vertexIndex.Add(1);	// 左下
		vertexIndex.Add(2);	// 右上
		vertexIndex.Add(2);	// 右上
		vertexIndex.Add(1);	// 左下
		vertexIndex.Add(3);	// 右下
		
		// uv
		FVector2D UvStartPoint = FVector2D(0.f, 0.f);
		if (i == 0)										// メッシュ1枚目は+で確定
		{
			UvStartPoint = FVector2D(0.5f, 2.f / 3.f);
		}
		else
		{
			int meshNum = _score / (int)(FMath::Pow(10.0f, (float)(ScoreDigits - i))) % 10;	// 表示したい数字

			if (meshNum <= 3)							// 表示したいのが0～3
			{
				UvStartPoint.X = (float)meshNum * 0.25f;
			}
			else if (meshNum <= 7)						// 表示したいのが4～7
			{
				UvStartPoint = FVector2D((float)(meshNum - 4) * 0.25f, 1.f / 3.f);
			}
			else										// 表示したいのが8～9
			{
				UvStartPoint = FVector2D((float)(meshNum - 8) * 0.25f, 2.f / 3.f);
			}
		}
		
		uv.Add(FVector2D(UvStartPoint.X, UvStartPoint.Y));	// 左上
		uv.Add(FVector2D(UvStartPoint.X, UvStartPoint.Y + (1.f / 3.f)));	// 左下
		uv.Add(FVector2D(UvStartPoint.X + 0.25f, UvStartPoint.Y));	// 右上
		uv.Add(FVector2D(UvStartPoint.X + 0.25f, UvStartPoint.Y + (1.f / 3.f)));	// 右下

		// メッシュにマテリアルをセットする
		FString materialPath = "/Game/UI/Textures/ScoreNumberMat.ScoreNumberMat";
		UMaterialInterface* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *materialPath));
		pMesh->SetMaterial(i, pMaterial);

		// メッシュを作成して表示
		pMesh->CreateMeshSection_LinearColor(i, vertex, vertexIndex, normal, uv, vertexColor, tangent, false);
	}

	// タイマースタート
	bDisplay = true;
}