// ばけものチーム
// 隠し妖怪が見つけられ際の処理

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "SecretBakemono.generated.h"

class UCheckInScreen;

UCLASS()
class BAKEMONOBAKARI_API ASecretBakemono : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretBakemono();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//発見しているか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsFound() const { return m_FoundAnime; }

	// 移動しているか
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		bool IsMoveing() const { return m_MoveAnime; }
	UFUNCTION(BlueprintCallable, Category = "My Functions")
		void MoveStart() { m_MoveAnime = true; }

private:
	// オーバーラップ関数
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 20.f))
		float m_MoveSpeedY;					// 進行方向のスピード

	UPROPERTY(EditAnywhere)
		float m_Score;						// 見つけた際の追加スコア

private:
	USkeletalMeshComponent* m_pEnemyMesh;	// メッシュ
	UCheckInScreen* m_pCheckInScreen;		// 画面外か判別する
	bool m_FoundAnime;						//発見アニメーション
	bool m_MoveAnime;						// 移動アニメーション
};
