// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//�X�V����
// 2021/04/23 �X�V�ҁF�@�U���ɂ��ď���
// 2021/05/04 �X�V�ҁF�@�ړ��ɂ��ď���
// 2021/05/14 �X�V�ҁF�@���ċ������L�т�i�W�����v�j
// 2021/05/17 �X�V�ҁF�@�_���[�W���󂯂���m�b�N�o�b�N���鏈��
// 2021/05/19 �X�V�ҁF�@���G���ԃt���O

#include "BakemonoBakariCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABakemonoBakariCharacter::ABakemonoBakariCharacter()
	: IsAttack(false)       //�U���t���O 4/23
	, IsJump(false)         //�W�����v�t���O 4/23
	, IsCrouch(false)       //���Ⴊ�݃t���O 4/23
	, IsDamage(false)       //�_���[�W�t���O 5/9
	, IsFaceRight(true)     //�E�����t���O 5/10
	, IsHanging(false)      //���ċ����L�уt���O5/14
	, IsInvincible(false)   //���G���ԃt���O5/19
	, IsOverlapping(false)  //�I�u�W�F�N�g�ƐڐG���Ă���t���O 5/19
	, EnemyLocation(0.0)    //�G�̐����ʒu 5/19
{
	//���t���[���A�N���X��Tick()���ĂԂ��ǂ��������߂�t���O
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
	GetCharacterMovement()->JumpZVelocity = 1000.f; //�W�����v�ʒ���
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Overlap�e�X�g(TriggerVolume/Damage)5/14
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

	//Overlap���Ă��鎞�_���[�W���󂯂鏈�� 5/20
	if (IsOverlapping == true && IsInvincible == false)
	{
		TakeDamage(10.0);			//�_���[�W�v�Z
		IsDamage = true;			//�_���[�W�󂯂Ă���
		IsInvincible = true;		//���G���Ԃɓ���
		KnockBack(EnemyLocation);	//�v���C���[���m�b�N�o�b�N�����
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
	if (IsAttack == false && IsDamage == false) //�U�����Ă��鎞���邢�̓_���[�W���󂯂Ă��鎞�Ɉړ��ł��Ȃ��@4/23
	{
		//FRotator StartRotation;
		//FRotator EndRotation;
		//StartRotation = GetActorRotation();
		//EndRotation = GetActorRotation();
		//EndRotation.Yaw = StartRotation.Yaw;

		if (Value > 0.0)     //�v���C���[���K���ړ������ɉ�]���鏈�� 5/4
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

//���ċ������L�т�֐�5/14
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

//�U�����͊֐� 4/23
void ABakemonoBakariCharacter::Attack()
{
	IsAttack = true;
}

//�_���[�W���󂯂鏈�� 5/7
void ABakemonoBakariCharacter::TakeDamage(float _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp < 0)
	{
		m_info.hp = 0;
	}
}

//�_���[�W���󂯂���m�b�N�o�b�N���鏈�� 5/17
void ABakemonoBakariCharacter::KnockBack(float _enemylocation)
{
	//�v���C���[�̑��x��0�ɂ��� 5/24
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);

	//�v���C���[�ɓG�̈ʒu�ɂ���ăm�b�N�o�b�N�t�H�[�X��^����
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

//Overlap�e�X�g5/14
void ABakemonoBakariCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//Overlap����I�u�W�F�N�g�͓G�̏ꍇ
		if (OtherActor->ActorHasTag("Enemy"))
		{
			IsOverlapping = true;								//Overlap���Ă���
			EnemyLocation = OtherActor->GetActorLocation().Y;	//Overlap����G�̈ʒu���擾
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
//Overlap�e�X�g5/14
void ABakemonoBakariCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}

	if (OtherActor->ActorHasTag("Enemy"))
	{
		//IsDamage = false;
		IsOverlapping = false;	//Overlap���Ă��Ȃ�
	}
}