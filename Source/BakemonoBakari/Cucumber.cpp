// ばけものチーム


#include "Cucumber.h"
// Sets default values
ACucumber::ACucumber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACucumber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACucumber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsMove)
	{
		if (m_IsRight) 
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, -m_Speed, 0.0f));
			AddActorLocalRotation(FRotator(0.0f, m_Speed, 0.0f));
		}
		else 
		{
			SetActorLocation(GetActorLocation() + FVector(0.0f, m_Speed, 0.0f));
			AddActorLocalRotation(FRotator(0.0f, -m_Speed, 0.0f));
		}
	}
}
// 移動設定を行う---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ACucumber::MoveStart(float _speed,bool _right)
{
	m_Speed = _speed;
	m_IsRight = _right;
	m_IsMove = true;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
