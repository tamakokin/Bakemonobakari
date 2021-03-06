// ばけものチーム
// EnemyScore.cpp
// 概要		：敵を撃破時に数秒間出現するUIメッセージ
// 作成者	：伴野慧
// 作成日	：2021/9/06
// 更新履歴	：2021/9/08	1枚のテクスチャに複数書かれた数字を切り出して使用するのではなく、数字ごと別々のテクスチャを使用するように仕様変更
// 更新履歴	：2021/9/10	出現の際に動きをつける（伴野）
//			：2021/9/10	10のべき乗の数の際戦闘の位が表示されないバグ修正（伴野）


#include "EnemyScore.h"
#include "ProceduralMeshComponent.h"

class UMaterial;

// Sets default values
AEnemyScore::AEnemyScore()
	: bDisplay(false)
	, DisplayTimer(0.f)
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

		// 出現直後
		if (DisplayTimer < ScrollTime)
		{
			SetActorScale3D(FVector(1.f, 1.f, DisplayTimer / ScrollTime));
		}
		else if (DisplayTimer > DisplayTime + ScrollTime)
		{
			Destroy();
		}
		
		// 消える直前
		else if (DisplayTimer > DisplayTime)
		{
			SetActorScale3D(FVector(1.f, 1.f, 1.f - ((DisplayTimer - DisplayTime) / ScrollTime)));
		}
		else
		{
			SetActorScale3D(FVector(1.f));
		}
	}
}

void AEnemyScore::SetScore(int _score)
{

	// スコアの桁数をカウント
	int ScoreDigits = 1;
	while ( _score >= (int)(FMath::Pow(10.0f, (float)ScoreDigits)) )
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
		
		uv.Add(FVector2D(0.1f, 0.f));	// 左上
		uv.Add(FVector2D(0.1f, 1.f));	// 左下
		uv.Add(FVector2D(0.9f, 0.f));	// 右上
		uv.Add(FVector2D(0.9f, 1.f));	// 右下

		// メッシュにマテリアルをセットする
		FString materialPath = "/Game/UI/Textures/Number/";

		// uv
		if (i == 0)										// メッシュ1枚目は+で確定
		{
			materialPath += "+_Mat.+_Mat";
		}
		else
		{
			int meshNum = _score / (int)(FMath::Pow(10.0f, (float)(ScoreDigits - i))) % 10;	// 表示したい数字

			materialPath += FString::FromInt(meshNum) + "_Mat." + FString::FromInt(meshNum) + "_Mat";
		}

		UMaterialInterface* pMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *materialPath));
		pMesh->SetMaterial(i, pMaterial);

		// メッシュを作成して表示
		pMesh->CreateMeshSection_LinearColor(i, vertex, vertexIndex, normal, uv, vertexColor, tangent, false);
	}

	// タイマースタート
	bDisplay = true;
}