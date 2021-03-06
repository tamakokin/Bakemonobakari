// ばけものチーム
// 隠し妖怪が見つけられ際の処理
// 更新履歴	：2021/9/10	スコア獲得時の表示を出す（伴野）

#include "SecretBakemono.h"
#include "MyGameInstance.h"
#include "EnemyScore.h"
#include "CheckInScreen.h"

// Sets default values
ASecretBakemono::ASecretBakemono():
	m_MoveAnime(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 画面何にいるかを判別するコンポーネントの生成
	m_pCheckInScreen = CreateDefaultSubobject<UCheckInScreen>(TEXT("CheckInScreen"));
}

// Called when the game starts or when spawned
void ASecretBakemono::BeginPlay()
{
	Super::BeginPlay();

	// メッシュを探す
	m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));

	//	オーバーラップ接触し始めた時に呼ばれるイベント関数を記録
	if (m_pEnemyMesh)
	{
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &ASecretBakemono::OnOverlapBegin);
		UE_LOG(LogTemp, Warning, TEXT("AAA"));
	}
	else
	{
		// メッシュを探す
		m_pEnemyMesh = Cast<USkeletalMeshComponent>(GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		m_pEnemyMesh->OnComponentBeginOverlap.AddDynamic(this, &ASecretBakemono::OnOverlapBegin);

	}
}

// Called every frame
void ASecretBakemono::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 移動状態の時
	if (m_MoveAnime)
	{
		SetActorLocation(FVector(0.0f, GetActorLocation().Y + m_MoveSpeedY, GetActorLocation().Z));

		// 画面外に出たら消す
		if (!m_pCheckInScreen->Check(GetActorLocation())) 
		{
			Destroy();
		}
	}
}


// オーバーラップ関数---------------------------------------------------------------------------------------------------------------------------------------------------
UFUNCTION() void ASecretBakemono::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	// アクターのポインターがぬるでなければ
	if (OtherActor != NULL)
	{
		// 接触したアクターのタグがプレイヤーであれば
		if (OtherActor->ActorHasTag("PlayerCharacter"))
		{
			// 初めて当たった際
			if (!m_FoundAnime)
			{
				// スコアを加算する
				Cast<UMyGameInstance>(GetGameInstance())->AddScore(m_Score, SCORE_TYPE::SCORE_TYPE_SECRET);
				m_FoundAnime = true;

				// スコア獲得時の表示を出す
				AEnemyScore* m_pEnemyScore;
				FString ScorePath = "/Game/Blueprints/EnemyScore.EnemyScore_C";
				TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(ScorePath)).LoadSynchronous();

				m_pEnemyScore = GetWorld()->SpawnActor<AEnemyScore>(subClass, GetActorLocation(), FRotator(0.f, 180.f, 0.f));
				m_pEnemyScore->SetScore(m_Score);
			}
		}	
	}
}
// ---------------------------------------------------------------------------------------------------------------------------------------------------
