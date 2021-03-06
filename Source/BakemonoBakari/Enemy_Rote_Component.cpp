// ばけものチーム

#include "Enemy_Rote_Component.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyActor.h"

// Sets default values for this component's properties
UEnemy_Rote_Component::UEnemy_Rote_Component() :
	m_RoteSpeed(15.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemy_Rote_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_pEnemy = Cast<AEnemyActor>(GetOwner());

	// プレイヤーの探索
	SerchPlayer();
}


// Called every frame
void UEnemy_Rote_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!m_pEnemy)
	{
		m_pEnemy = Cast<AEnemyActor>(GetOwner());
		return;
	}

	// プレイヤーの探索
	SerchPlayer();

	FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

	// エネミーを180度回転させる
	if (m_RoteCount > 0)
	{
		m_RoteCount++;

		tempEnemyRotation.Yaw += m_RoteSpeed;

		// 回転が270を超えないようにする
		if (tempEnemyRotation.Yaw >= 270)
		{
			tempEnemyRotation.Yaw = 90.0f;
		}

		m_pEnemy->SetActorRotation(FRotator(tempEnemyRotation.Pitch, tempEnemyRotation.Yaw, tempEnemyRotation.Roll));

		UE_LOG(LogTemp, Warning, TEXT("m_RoteCount%f"), tempEnemyRotation.Yaw);

		// 指定角度回転をしたらカウントを0にする
		if (m_RoteSpeed * m_RoteCount > m_RoteTarget)
		{
			m_RoteCount = 0;
		}
	}
	// 移動を行う
}
// プレイヤーの探索を行う------------------------------------------------------------------------------------------------------------------------------------
void UEnemy_Rote_Component::SerchPlayer()
{
	// 一回だけ
	if (m_pPlayerActor)return;

	// プレイヤーを探す
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	if (actors.Num() > 0)
	{
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			if (pActor->ActorHasTag("PlayerCharacter"))
			{
				m_pPlayerActor = pActor;
				break;
			}
		}
	}
}
// --------------------------------------------------------------------------------------------------------------------------------------------

// プレイヤーの方向を向く---------------------------------------------------------------------------------------------------------------------------------
void UEnemy_Rote_Component::EnemyRote()
{
	if (m_RoteCount == 0)
	{
		if (!m_pEnemy)
		{
			m_pEnemy = Cast<AEnemyActor>(GetOwner());
			return;
		}
		FVector tempEnemyPosition = m_pEnemy->GetActorLocation();
		FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

		FVector tempPlayerCharacterPosition = FVector::ZeroVector;
		if (m_pPlayerActor != NULL)
		{
			tempPlayerCharacterPosition = m_pPlayerActor->GetActorLocation();
		}

		// プレイヤーが自分の向いている向きの反対方向にいるか確認
		if (tempPlayerCharacterPosition.Y >= tempEnemyPosition.Y)
		{
			if (tempEnemyRotation.Yaw < 0)
			{
				m_RoteTarget = 180.0f;
				m_RoteCount = 1;
			}
		}
		else if (tempPlayerCharacterPosition.Y < tempEnemyPosition.Y)
		{
			if (tempEnemyRotation.Yaw > 0)
			{
				m_RoteTarget = 180.0f;
				m_RoteCount = 1;
			}
		}
	}
}
// ----------------------------------------------------------------------------------------------------------------------------

// 向きを反転する---------------------------------------------------------------------------------------------------------------------------------------------------
void UEnemy_Rote_Component::TurnRote(float _rote)
{
	if (m_RoteCount == 0)
	{
		if (!m_pEnemy)
		{
			m_pEnemy = Cast<AEnemyActor>(GetOwner());
			return;
		}
		FRotator tempEnemyRotation = m_pEnemy->GetActorRotation();

		m_RoteTarget = _rote;
		m_RoteCount = 1;
	}
}
// ----------------------------------------------------------------------------------------------------------------------------

// プレイヤーの座標を返す
FVector UEnemy_Rote_Component::GetPlayerPos()
{
	if (m_pPlayerActor)
	{
		return m_pPlayerActor->GetActorLocation();
	}

	return FVector::ZeroVector;
}

