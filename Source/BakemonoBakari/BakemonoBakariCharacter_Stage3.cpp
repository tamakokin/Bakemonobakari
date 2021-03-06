// ばけものチーム
// 更新履歴
// 2021/07/28 更新者：陳　作成

#include "BakemonoBakariCharacter_Stage3.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABakemonoBakariCharacter_Stage3::ABakemonoBakariCharacter_Stage3()
	: IsAttack(false)       //攻撃フラグ 7/28
	, IsJump(false)         //ジャンプフラグ 7/28
	, IsCrouch(false)       //しゃがみフラグ 7/29
	, IsDamage(false)       //ダメージフラグ 7/29
	, IsInvincible(false)   //無敵時間フラグ 7/29
	, IsDead(false)			//死亡フラグ 7/29
	, IsEnemyContact(false) //敵と接触しているフラグ 5/19
	, IsFaceRight(true)     //右方向フラグ 7/28
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 1.6f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 600.f; //ジャンプ量調整
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MaxFlySpeed = 700.f;

	//Overlapテスト(TriggerVolume/Damage)5/14
	OverlapComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Overlap"));
	OverlapComponent->InitCapsuleSize(20.f, 88.0f);
	OverlapComponent->SetupAttachment(RootComponent);

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ABakemonoBakariCharacter_Stage3::OnOverlapBegin);
	OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ABakemonoBakariCharacter_Stage3::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ABakemonoBakariCharacter_Stage3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABakemonoBakariCharacter_Stage3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Overlapしている時ダメージを受ける処理 7/28
	if (IsEnemyContact == true && IsInvincible == false && IsDead == false)
	{
		TakeDamage(10.0);			//ダメージ計算
		IsDamage = true;			//ダメージ受けている
		IsInvincible = true;		//無敵時間に入る
	}
}

// Called to bind functionality to input
void ABakemonoBakariCharacter_Stage3::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABakemonoBakariCharacter_Stage3::MoveRight);
	//PlayerInputComponent->BindAxis("MoveUp", this);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABakemonoBakariCharacter_Stage3::Attack);
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABakemonoBakariCharacter_Stage3::OnFire);
}

void ABakemonoBakariCharacter_Stage3::MoveRight(float Value)
{
	if (IsDamage == false && IsDead == false)
	{
		//FRotator StartRotation;
		//FRotator EndRotation;
		//StartRotation = GetActorRotation();
		//EndRotation = GetActorRotation();
		//EndRotation.Yaw = StartRotation.Yaw;
		if (Value > 0.0)
		{
			//NewRotation() = FMath::Lerp(FRotator(0.f, 180.f, 0.f), FRotator(0.f, 0.f, 0.f), );
			//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
			//SetActorRotation(FRotator(0.f, 0.f, -90.f));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("right"));
			// add movement in that direction
			AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
			IsFaceRight = true;
		}
		else if (Value < 0.0)
		{
			//SetActorRotation(FMath::RInterpTo(StartRotation, EndRotation, GetWorld()->GetDeltaSeconds(), 500));
			//SetActorRotation(FRotator(0.f, 0.f, 90.f));
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("left"));
			// add movement in that direction
			AddMovementInput(FVector(0.f, 1.f, 0.f), Value);
			IsFaceRight = false;
		}
	}
		
}

//攻撃入力関数 7/28
void ABakemonoBakariCharacter_Stage3::Attack()
{
	//if (IsInputFadeIn == false)	//フェードイン時
	//{
	if (!IsDamage && !IsDead) 
	{
		IsAttack = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "shoot");
	}
	//}

	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			//if (bUsingMotionControllers)
			//{
			const FRotator SpawnRotation = GetActorRotation();//VR_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = GetActorLocation();// VR_MuzzleLocation->GetComponentLocation();
			//World->SpawnActor<APlayerBullet>(ProjectileClass, SpawnLocation, SpawnRotation);
			//}
			//else
			//{
				//const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				//FActorSpawnParameters ActorSpawnParams;
				//ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				//World->SpawnActor<ABakemonoBakariProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			//}
		}
	}
}

//ダメージを受ける処理 7/29
void ABakemonoBakariCharacter_Stage3::TakeDamage(float _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp <= 0)
	{
		m_info.hp = 0;
		//HPが0になると死亡状態になる
		IsDead = true;
	}
}

//Overlapテスト7/29
void ABakemonoBakariCharacter_Stage3::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//Overlapするオブジェクトは敵の場合
		if (OtherActor->ActorHasTag("Enemy") && IsInvincible == false)
		{
			IsEnemyContact = true;								//Overlapしている
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


//Overlapテスト7/29
void ABakemonoBakariCharacter_Stage3::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}

	if (OtherActor->ActorHasTag("Enemy"))
	{
		//IsDamage = false;
		IsEnemyContact = false;	//Overlapしていない
	}
}
