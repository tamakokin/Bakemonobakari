// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//更新履歴
// 2021/04/23 更新者：陳　攻撃について処理
// 2021/05/04 更新者：陳　移動について処理
// 2021/05/14 更新者：陳　飛翔距離が伸びる（ジャンプ）
// 2021/05/17 更新者：陳　ダメージを受けたらノックバックする処理
// 2021/05/19 更新者：陳　無敵時間フラグ

#include "BakemonoBakariCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABakemonoBakariCharacter::ABakemonoBakariCharacter()
	: IsAttack(false)       //攻撃フラグ 4/23
	, IsJump(false)         //ジャンプフラグ 4/23
	, IsCrouch(false)       //しゃがみフラグ 4/23
	, IsDamage(false)       //ダメージフラグ 5/9
	, IsFaceRight(true)     //右方向フラグ 5/10
	, IsHanging(false)      //飛翔距離伸びフラグ5/14
	, IsInvincible(false)   //無敵時間フラグ5/19
	, IsOverlapping(false)  //オブジェクトと接触しているフラグ 5/19
	, EnemyLocation(0.0)    //敵の水平位置 5/19
{
	//毎フレーム、クラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//// Create a camera boom attached to the root (capsule)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	//CameraBoom->bDoCollisionTest = false;
	//CameraBoom->TargetArmLength = 800.f;
	//CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	//CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	//// Create a camera and attach to boom
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f; //ジャンプ量調整
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Overlapテスト(TriggerVolume/Damage)5/14
	OverlapComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Overlap"));
	OverlapComponent->InitCapsuleSize(42.f, 96.0f);
	OverlapComponent->SetupAttachment(RootComponent);

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ABakemonoBakariCharacter::OnOverlapBegin);
	OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ABakemonoBakariCharacter::OnOverlapEnd);
	
}

void ABakemonoBakariCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABakemonoBakariCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Hang();

	//Overlapしている時ダメージを受ける処理 5/20
	if (IsOverlapping == true && IsInvincible == false)
	{
		TakeDamage(10.0);			//ダメージ計算
		IsDamage = true;			//ダメージ受けている
		IsInvincible = true;		//無敵時間に入る
		KnockBack(EnemyLocation);	//プレイヤーがノックバックされる
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABakemonoBakariCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABakemonoBakariCharacter::MoveRight);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABakemonoBakariCharacter::Attack);

	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ABakemonoBakariCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ABakemonoBakariCharacter::TouchStopped);
}

void ABakemonoBakariCharacter::MoveRight(float Value)
{
	if (IsAttack == false && IsDamage == false) //攻撃している時あるいはダメージを受けている時に移動できない　4/23
	{
		//FRotator StartRotation;
		//FRotator EndRotation;
		//StartRotation = GetActorRotation();
		//EndRotation = GetActorRotation();
		//EndRotation.Yaw = StartRotation.Yaw;

		if (Value > 0.0)     //プレイヤーが必ず移動方向に回転する処理 5/4
		{
			//NewRotation() = FMath::Lerp(FRotator(0.f, 180.f, 0.f), FRotator(0.f, 0.f, 0.f), );
			//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
			SetActorRotation(FRotator(0.f, -90.f, 0.f));
			// add movement in that direction
			AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
			IsFaceRight = true;
		}
		else
		{
			if (Value < 0.0)
			{
				//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
				SetActorRotation(FRotator(0.f, 90.f, 0.f));
				// add movement in that direction
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
				IsFaceRight = false;
			}
		}
	}
}

//void ABakemonoBakariCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	// jump on any touch
//	Jump();
//}
//
//void ABakemonoBakariCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	StopJumping();
//}

//飛翔距離が伸びる関数5/14
void ABakemonoBakariCharacter::Hang()
{
	if (IsHanging == true)
	{
		GetCharacterMovement()->GravityScale = 2.5f;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 4.0f;
	}
	
}

//攻撃入力関数 4/23
void ABakemonoBakariCharacter::Attack()
{
	IsAttack = true;
}

//ダメージを受ける処理 5/7
void ABakemonoBakariCharacter::TakeDamage(float _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp < 0)
	{
		m_info.hp = 0;
	}
}

//ダメージを受けたらノックバックする処理 5/17
void ABakemonoBakariCharacter::KnockBack(float _enemylocation)
{
	//プレイヤーの速度を0にする 5/24
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);

	//プレイヤーに敵の位置によってノックバックフォースを与える
	if (_enemylocation <= UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation().Y)
	{		
		LaunchCharacter(FVector(0.0f, 800.f, 800.f), false, false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "KnockBackToleft");
	}
	else
	{
		LaunchCharacter(FVector(0.0f, -800.f, 800.f), false, false);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "KnockBackToRight");
	}
}

//Overlapテスト5/14
void ABakemonoBakariCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//Overlapするオブジェクトは敵の場合
		if (OtherActor->ActorHasTag("Enemy"))
		{
			IsOverlapping = true;								//Overlapしている
			EnemyLocation = OtherActor->GetActorLocation().Y;	//Overlapする敵の位置を取得
		}
		if (GEngine)
		{
			FString TheFloatStr = FString::SanitizeFloat(m_info.hp);
			//FString TheFloatStr_2= FString::SanitizeFloat(OtherActor->GetActorLocation().Y);
			//FString TheFloatStr_3 = FString::SanitizeFloat(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation().Y);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *TheFloatStr);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "true");
		}

	}
}
//Overlapテスト5/14
void ABakemonoBakariCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}

	if (OtherActor->ActorHasTag("Enemy"))
	{
		//IsDamage = false;
		IsOverlapping = false;	//Overlapしていない
	}
}