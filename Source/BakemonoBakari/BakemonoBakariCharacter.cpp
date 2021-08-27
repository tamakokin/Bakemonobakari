// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//�X�V����
// 2021/04/23 �X�V�ҁF�@�U���ɂ��ď���
// 2021/05/04 �X�V�ҁF�@�ړ��ɂ��ď���
// 2021/05/14 �X�V�ҁF�@���ċ������L�т�i�W�����v�j
// 2021/05/17 �X�V�ҁF�@�_���[�W���󂯂���m�b�N�o�b�N���鏈��
// 2021/05/19 �X�V�ҁF�@���G���ԃt���O
// 2021/05/26 �X�V�ҁF�@HP��0�ɂȂ������̏���
// 2021/06/07 �X�V�ҁF����@�V�[���J�n���̃t�F�[�h�C���̍Œ��͓��͂��󂯕t���Ȃ�
// 2021/06/07 �X�V�ҁF����@�v���C���[���S���̃��X�|�[�������̒ǉ�
// 2021/07/05 �X�V�ҁF����@��q�̏�艺��@�\�̒ǉ�
// 2021/07/05 �X�V�ҁF����@���E���͂��󂯎�����ہA��x�ϐ��ɕۑ�����悤�ύX
// 2021/07/08 �X�V�ҁF����@��q��o��؂����ۂ̏�����ǉ�
// 2021/08/19 �X�V�ҁF��c�@�ړ��U�����̊�����ǉ�
// 2021/08/20 �X�V�ҁF�R�c�@�񕜃A�C�e�����擾������hp�𑝂₷����

#include "BakemonoBakariCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#define PLAYER_HEIGHT 96.f
#define PLAYER_WIDTH 42.f

ABakemonoBakariCharacter::ABakemonoBakariCharacter()
	: IsAttack(false)			// �U���t���O 4/23
	, IsJump(false)				// �W�����v�t���O 4/23
	, IsCrouch(false)			// ���Ⴊ�݃t���O 4/23
	, IsInputFadeIn(true)		// �t�F�[�h�C�����̓��͕s�t���O 6/7
	, IsDamage(false)			// �_���[�W�t���O 5/9
	, IsFaceRight(true)			// �E�����t���O 5/10
	, IsLadder(false)			// ��q�t���O 7/5����
	, IsLadderClimb(false)		// ��q��o��؂����t���O 7/8����
	, LadderDir(ELadderDirection::LD_None)		// �͂�ł����q�̌��� 7/5����
	, IsHanging(false)			// ���ċ����L�уt���O5/14
	, IsInvincible(false)		// ���G���ԃt���O5/19
	, IsDead(false)				// ���S�t���O5/26
	, IsFallDead(false)			// �]�����S�t���O 5/28
	, IsEnemyContack(false)		// �I�u�W�F�N�g�ƐڐG���Ă���t���O 5/19
	, EnemyLocation(0.0)		// �G�̐����ʒu 5/19
	, m_Horizontal(0.f)			// ���������̓��͒l�ێ�
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
	GetCharacterMovement()->JumpZVelocity = 1400.f; //�W�����v�ʒ���
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
	m_ReStartPos = GetActorLocation();

}

void ABakemonoBakariCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsLadder)
	{
		Hang();
	}

	if (IsLadderClimb)
	{
		ClimbLadder(DeltaTime);
	}

	// ���E���͂𔽉f������ 7/5����
	MoveRight(InputValue.X);
	MoveUp(InputValue.Y);

	//�]�����S�̏��� 5/28
	if (IsFallDead == true)
	{
	TakeDamage(100.0);			//�_���[�W�v�Z
	//IsDamage = true;			//�_���[�W�󂯂Ă���
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
	}
	//Overlap���Ă��鎞�_���[�W���󂯂鏈�� 5/20
	else if (IsEnemyContack == true && IsInvincible == false && IsDead == false)
	{
		TakeDamage(10.0);			//�_���[�W�v�Z
		IsDamage = true;			//�_���[�W�󂯂Ă���
		IsInvincible = true;		//���G���Ԃɓ���
		KnockBack(EnemyLocation);	//�v���C���[���m�b�N�o�b�N�����
	}
	//Overlap���Ă��鎞�񕜏��� 8/20
	if (IsRecoveryContack == true)
	{
		TakeRecovery(10.0f);
	}

	SetActorLocation(FVector(0.0f, GetActorLocation().Y, GetActorLocation().Z));
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABakemonoBakariCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABakemonoBakariCharacter::InputRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ABakemonoBakariCharacter::InputUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABakemonoBakariCharacter::Attack);

	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ABakemonoBakariCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ABakemonoBakariCharacter::TouchStopped);
}


// ���E���͂̒l���󂯎���ĕϐ��ɕۑ�����֐� 7/5����
void ABakemonoBakariCharacter::InputRight(float Value)
{
	InputValue.X = Value;
	m_Horizontal = Value;
}

// �㉺���͂̒l���󂯎���ĕϐ��ɕۑ�����֐� 7/5����
void ABakemonoBakariCharacter::InputUp(float Value)
{
	InputValue.Y = Value;
}

void ABakemonoBakariCharacter::MoveRight(float Value)
{
	if (IsInputFadeIn == false)	//�t�F�[�h�C�����͈ړ��ł��Ȃ�
	{
		if (IsAttack == false && IsDamage == false && IsDead == false) //�U�����Ă��鎞���邢�̓_���[�W���󂯂Ă��鎞�Ɉړ��ł��Ȃ��@4/23
		{
			//FRotator StartRotation;
			//FRotator EndRotation;
			//StartRotation = GetActorRotation();
			//EndRotation = GetActorRotation();
			//EndRotation.Yaw = StartRotation.Yaw;
			if (!IsLadder)		// ��q��͂�ł���Ƃ��͈ړ��ł��Ȃ�
			{
				// ��q��͂�ł��Ȃ�����q�̋߂��ɂ���Ƃ�
				if (LadderDir != ELadderDirection::LD_None)
				{
					// ��q�Ɍ������Ĉړ�������
					if ((LadderDir == ELadderDirection::LD_Right && Value > 0.f) || (LadderDir == ELadderDirection::LD_Left && Value < 0.f))
					{
						// ��q��͂�
						GraspLadder();
					}
				}

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
		else if (IsAttack == true && IsJump)
		{
			if (Value > 0.0)     //�v���C���[���K���ړ������ɉ�]���鏈�� 5/4
			{
				//NewRotation() = FMath::Lerp(FRotator(0.f, 180.f, 0.f), FRotator(0.f, 0.f, 0.f), );
				//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
				SetActorRotation(FRotator(0.f, -90.f, 0.f));
				// add movement in that direction
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value+1.5f);
				IsFaceRight = true;
			}
			else
			{
				if (Value < 0.0)
				{
					//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
					SetActorRotation(FRotator(0.f, 90.f, 0.f));
					// add movement in that direction
					AddMovementInput(FVector(0.f, -1.f, 0.f), Value-1.5f);
					IsFaceRight = false;
				}
			}
		}

		// �U����̊���
		if (IsAttack)
		{
			if (Value > 0.0)     //�v���C���[���K���ړ������ɉ�]���鏈�� 5/4
			{
				//NewRotation() = FMath::Lerp(FRotator(0.f, 180.f, 0.f), FRotator(0.f, 0.f, 0.f), );
				//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
				SetActorRotation(FRotator(0.f, -90.f, 0.f));
				// add movement in that direction
				AddMovementInput(FVector(0.f, -1.f, 0.f), 0.1f);
				IsFaceRight = true;
			}
			else
			{
				if (Value < 0.0)
				{
					//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
					SetActorRotation(FRotator(0.f, 90.f, 0.f));
					// add movement in that direction
					AddMovementInput(FVector(0.f, -1.f, 0.f), -0.1f);
					IsFaceRight = false;
				}
			}
		}
	}
}

// �㉺���͂��ꂽ�ꍇ�̏��� 7/5����
void ABakemonoBakariCharacter::MoveUp(float Value)
{
	if (IsInputFadeIn == false)	//�t�F�[�h�C�����͈ړ��ł��Ȃ�
	{
		if (IsLadder)	// ��q�ɐڐG���Ă���Ƃ��̂ݏ㉺�ړ��ł���
		{
			if (Value > 0.0f)
			{
				AddMovementInput(FVector(0.f, 0.f, 1.f), Value);
			}
			else if (Value < 0.0f)
			{
				AddMovementInput(FVector(0.f, 0.f, 1.f), Value);
			}
			else
			{
				GetCharacterMovement()->Velocity = FVector(0.f,0.f,0.f);
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
	if (IsInputFadeIn == false)	//�t�F�[�h�C����
	{
		if (!IsDamage && !IsDead && !IsLadder) {
			IsAttack = true;
		}
	}
}

//�_���[�W���󂯂鏈�� 5/7
void ABakemonoBakariCharacter::TakeDamage(float _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp <= 0)
	{
		m_info.hp = 0;
		//HP��0�ɂȂ�Ǝ��S��ԂɂȂ�
		IsDead = true;
	}
}

//�񕜏��� 8/20
void ABakemonoBakariCharacter::TakeRecovery(float _recovery)
{
	if (m_info.hp < 100.0)
	{
		m_info.hp += _recovery;
	}
	IsRecoveryContack = false;
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
			IsEnemyContack = true;								//Overlap���Ă���
			EnemyLocation = OtherActor->GetActorLocation().Y;	//Overlap����G�̈ʒu���擾

			LeaveLadder();		// ��q���������
		}
		//Overlap����I�u�W�F�N�g�͉񕜃A�C�e���̏ꍇ
		else if (OtherActor->ActorHasTag("Recovery"))
		{
			IsRecoveryContack = true;
		}
		//Overlap����I�u�W�F�N�g�͗������̏ꍇ
		else if (OtherActor->ActorHasTag("Hole"))
		{
			IsEnemyContack = true;
			IsFallDead = true;
		}
		// ��q�̏ꍇ 7/5����
		else if (OtherActor->ActorHasTag("Ladder"))
		{
			// ���łɒ�q��͂�ł���ꍇ
			if (IsLadder)
			{
				// ���[�ɒH�蒅���ĐV���ȃI�[�o�[���b�v�����������̂ł����
				if (OtherComp->ComponentHasTag("LadderUnder"))
				{
					// ��q����~���
					LeaveLadder();
				}
				// ��[�ɒH�蒅���ĐV���ȃI�[�o�[���b�v�����������̂ł����
				else if (OtherComp->ComponentHasTag("LadderUpper"))
				{
					// �A�j���[�V������؂�ւ���

					// ��q��o��؂���
					IsLadderClimb = true;
					
					// ��q���痣���
					LeaveLadder();
				}
			}
			// �܂���q��͂�ł��Ȃ���ԂŁA��q�{�̂ɐG�ꂽ�Ƃ�
			else if (OtherComp->ComponentHasTag("LadderBase"))
			{
				GraspLadder();		// ��q��͂�

				// ��q�̌������L�^����
				if (GetActorLocation().Y > OtherActor->GetActorLocation().Y)
				{
					LadderDir = ELadderDirection::LD_Right;	// ��q�̓v���C���[�̉E�ɂ���
				}
				else
				{
					LadderDir = ELadderDirection::LD_Left;	// ��q�̓v���C���[�̍��ɂ���
				}

				UE_LOG(LogTemp, Warning, TEXT("Overlap Ladder"));
			}
		}
		// ���Ԓn�_�̏ꍇ
		else if (OtherActor->ActorHasTag("ReStart"))
		{
			m_ReStartPos = GetActorLocation();

			UE_LOG(LogTemp, Warning, TEXT("HIT"));
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
		IsEnemyContack = false;	//Overlap���Ă��Ȃ�
	}
	// ��q���痣�ꂽ�ꍇ 7/5����
	else if (OtherComp->ComponentHasTag("LadderBase"))
	{
		LadderDir = ELadderDirection::LD_None;		// �߂��ɒ�q�͂Ȃ�
		UE_LOG(LogTemp, Warning, TEXT("leave Ladder"));
	}
}

// ��q�����؂����Ƃ��̏��� 7/5����
void ABakemonoBakariCharacter::ClimbLadder(float DeltaTime)
{
	// �ړ�������߂�
	FVector actorMoveEndPoint = FVector(0.f, PLAYER_WIDTH*2, PLAYER_HEIGHT*2);
	if (LadderDir == ELadderDirection::LD_Right)
	{
		actorMoveEndPoint.Y *= -1.f;
	}
	AddActorWorldOffset(actorMoveEndPoint);

	IsLadderClimb = false;
}

void ABakemonoBakariCharacter::ReStart()
{
	SetActorLocation(m_ReStartPos);
	m_info.hp = 100.0;

	IsDamage = false;			// �_���[�W�󂯂Ă��Ȃ���Ԃ�
	IsDead = false;			// ���S�����畜��
	IsFallDead = false;
	IsInvincible = false;		// ���G���Ԃ̏I��
	IsEnemyContack = false;

	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
}

// ��q��͂ނƂ��̏��� 7/5����
void ABakemonoBakariCharacter::GraspLadder()
{
	IsLadder = true;
	GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);	// �v���C���[�̓������~�߂�
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);			// �d�͂̉e�����󂯂Ȃ��悤�ɂ���
}

// ��q���痣���Ƃ��̏��� 7/5����
void ABakemonoBakariCharacter::LeaveLadder()
{
	IsLadder = false;											// ��q��͂�ł��Ȃ�
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);		// �d�͂̉e�������ɖ߂�
}