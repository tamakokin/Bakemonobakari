// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//更新履歴
// 2021/04/23 更新者：陳　攻撃について処理
// 2021/05/04 更新者：陳　移動について処理

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
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ABakemonoBakariCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABakemonoBakariCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (IsAttack == false) //攻撃している時に移動できない　4/23
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
		}
		else
		{
			if (Value < 0.0)
			{
				//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
				SetActorRotation(FRotator(0.f, 90.f, 0.f));
				// add movement in that direction
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
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

//攻撃入力関数 4/23
void ABakemonoBakariCharacter::Attack()
{
	IsAttack = true;
}

//ダメージを受ける処理 5/7
void ABakemonoBakariCharacter::TakeDamage(int _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp < 0)
	{
		m_info.hp = 0;
	}
}