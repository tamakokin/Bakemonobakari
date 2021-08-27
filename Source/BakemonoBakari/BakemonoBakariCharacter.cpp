// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
//更新履歴
// 2021/04/23 更新者：陳　攻撃について処理
// 2021/05/04 更新者：陳　移動について処理
// 2021/05/14 更新者：陳　飛翔距離が伸びる（ジャンプ）
// 2021/05/17 更新者：陳　ダメージを受けたらノックバックする処理
// 2021/05/19 更新者：陳　無敵時間フラグ
// 2021/05/26 更新者：陳　HPが0になった時の処理
// 2021/06/07 更新者：伴野　シーン開始時のフェードインの最中は入力を受け付けない
// 2021/06/07 更新者：大金　プレイヤー死亡時のリスポーン処理の追加
// 2021/07/05 更新者：伴野　梯子の上り下り機能の追加
// 2021/07/05 更新者：伴野　左右入力を受け取った際、一度変数に保存するよう変更
// 2021/07/08 更新者：伴野　梯子を登り切った際の処理を追加
// 2021/08/19 更新者：上田　移動攻撃時の慣性を追加
// 2021/08/20 更新者：山田　回復アイテムを取得したらhpを増やす処理

#include "BakemonoBakariCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#define PLAYER_HEIGHT 96.f
#define PLAYER_WIDTH 42.f

ABakemonoBakariCharacter::ABakemonoBakariCharacter()
	: IsAttack(false)			// 攻撃フラグ 4/23
	, IsJump(false)				// ジャンプフラグ 4/23
	, IsCrouch(false)			// しゃがみフラグ 4/23
	, IsInputFadeIn(true)		// フェードイン時の入力不可フラグ 6/7
	, IsDamage(false)			// ダメージフラグ 5/9
	, IsFaceRight(true)			// 右方向フラグ 5/10
	, IsLadder(false)			// 梯子フラグ 7/5伴野
	, IsLadderClimb(false)		// 梯子を登り切ったフラグ 7/8伴野
	, LadderDir(ELadderDirection::LD_None)		// 掴んでいる梯子の向き 7/5伴野
	, IsHanging(false)			// 飛翔距離伸びフラグ5/14
	, IsInvincible(false)		// 無敵時間フラグ5/19
	, IsDead(false)				// 死亡フラグ5/26
	, IsFallDead(false)			// 転落死亡フラグ 5/28
	, IsEnemyContack(false)		// オブジェクトと接触しているフラグ 5/19
	, EnemyLocation(0.0)		// 敵の水平位置 5/19
	, m_Horizontal(0.f)			// 水平方向の入力値保持
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
	GetCharacterMovement()->JumpZVelocity = 1400.f; //ジャンプ量調整
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

	// 左右入力を反映させる 7/5伴野
	MoveRight(InputValue.X);
	MoveUp(InputValue.Y);

	//転落死亡の処理 5/28
	if (IsFallDead == true)
	{
	TakeDamage(100.0);			//ダメージ計算
	//IsDamage = true;			//ダメージ受けている
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
	}
	//Overlapしている時ダメージを受ける処理 5/20
	else if (IsEnemyContack == true && IsInvincible == false && IsDead == false)
	{
		TakeDamage(10.0);			//ダメージ計算
		IsDamage = true;			//ダメージ受けている
		IsInvincible = true;		//無敵時間に入る
		KnockBack(EnemyLocation);	//プレイヤーがノックバックされる
	}
	//Overlapしている時回復処理 8/20
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


// 左右入力の値を受け取って変数に保存する関数 7/5伴野
void ABakemonoBakariCharacter::InputRight(float Value)
{
	InputValue.X = Value;
	m_Horizontal = Value;
}

// 上下入力の値を受け取って変数に保存する関数 7/5伴野
void ABakemonoBakariCharacter::InputUp(float Value)
{
	InputValue.Y = Value;
}

void ABakemonoBakariCharacter::MoveRight(float Value)
{
	if (IsInputFadeIn == false)	//フェードイン時は移動できない
	{
		if (IsAttack == false && IsDamage == false && IsDead == false) //攻撃している時あるいはダメージを受けている時に移動できない　4/23
		{
			//FRotator StartRotation;
			//FRotator EndRotation;
			//StartRotation = GetActorRotation();
			//EndRotation = GetActorRotation();
			//EndRotation.Yaw = StartRotation.Yaw;
			if (!IsLadder)		// 梯子を掴んでいるときは移動できない
			{
				// 梯子を掴んでいないが梯子の近くにいるとき
				if (LadderDir != ELadderDirection::LD_None)
				{
					// 梯子に向かって移動したら
					if ((LadderDir == ELadderDirection::LD_Right && Value > 0.f) || (LadderDir == ELadderDirection::LD_Left && Value < 0.f))
					{
						// 梯子を掴む
						GraspLadder();
					}
				}

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
		else if (IsAttack == true && IsJump)
		{
			if (Value > 0.0)     //プレイヤーが必ず移動方向に回転する処理 5/4
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

		// 攻撃後の慣性
		if (IsAttack)
		{
			if (Value > 0.0)     //プレイヤーが必ず移動方向に回転する処理 5/4
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

// 上下入力された場合の処理 7/5伴野
void ABakemonoBakariCharacter::MoveUp(float Value)
{
	if (IsInputFadeIn == false)	//フェードイン時は移動できない
	{
		if (IsLadder)	// 梯子に接触しているときのみ上下移動できる
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
	if (IsInputFadeIn == false)	//フェードイン時
	{
		if (!IsDamage && !IsDead && !IsLadder) {
			IsAttack = true;
		}
	}
}

//ダメージを受ける処理 5/7
void ABakemonoBakariCharacter::TakeDamage(float _dmg)
{
	m_info.hp -= _dmg;

	if (m_info.hp <= 0)
	{
		m_info.hp = 0;
		//HPが0になると死亡状態になる
		IsDead = true;
	}
}

//回復処理 8/20
void ABakemonoBakariCharacter::TakeRecovery(float _recovery)
{
	if (m_info.hp < 100.0)
	{
		m_info.hp += _recovery;
	}
	IsRecoveryContack = false;
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
			IsEnemyContack = true;								//Overlapしている
			EnemyLocation = OtherActor->GetActorLocation().Y;	//Overlapする敵の位置を取得

			LeaveLadder();		// 梯子から手を放す
		}
		//Overlapするオブジェクトは回復アイテムの場合
		else if (OtherActor->ActorHasTag("Recovery"))
		{
			IsRecoveryContack = true;
		}
		//Overlapするオブジェクトは落し穴の場合
		else if (OtherActor->ActorHasTag("Hole"))
		{
			IsEnemyContack = true;
			IsFallDead = true;
		}
		// 梯子の場合 7/5伴野
		else if (OtherActor->ActorHasTag("Ladder"))
		{
			// すでに梯子を掴んでいる場合
			if (IsLadder)
			{
				// 下端に辿り着いて新たなオーバーラップが発生したのであれば
				if (OtherComp->ComponentHasTag("LadderUnder"))
				{
					// 梯子から降りる
					LeaveLadder();
				}
				// 上端に辿り着いて新たなオーバーラップが発生したのであれば
				else if (OtherComp->ComponentHasTag("LadderUpper"))
				{
					// アニメーションを切り替える

					// 梯子を登り切った
					IsLadderClimb = true;
					
					// 梯子から離れる
					LeaveLadder();
				}
			}
			// まだ梯子を掴んでいない状態で、梯子本体に触れたとき
			else if (OtherComp->ComponentHasTag("LadderBase"))
			{
				GraspLadder();		// 梯子を掴む

				// 梯子の向きを記録する
				if (GetActorLocation().Y > OtherActor->GetActorLocation().Y)
				{
					LadderDir = ELadderDirection::LD_Right;	// 梯子はプレイヤーの右にある
				}
				else
				{
					LadderDir = ELadderDirection::LD_Left;	// 梯子はプレイヤーの左にある
				}

				UE_LOG(LogTemp, Warning, TEXT("Overlap Ladder"));
			}
		}
		// 中間地点の場合
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
		IsEnemyContack = false;	//Overlapしていない
	}
	// 梯子から離れた場合 7/5伴野
	else if (OtherComp->ComponentHasTag("LadderBase"))
	{
		LadderDir = ELadderDirection::LD_None;		// 近くに梯子はない
		UE_LOG(LogTemp, Warning, TEXT("leave Ladder"));
	}
}

// 梯子を上り切ったときの処理 7/5伴野
void ABakemonoBakariCharacter::ClimbLadder(float DeltaTime)
{
	// 移動先を決める
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

	IsDamage = false;			// ダメージ受けていない状態に
	IsDead = false;			// 死亡時から復活
	IsFallDead = false;
	IsInvincible = false;		// 無敵時間の終了
	IsEnemyContack = false;

	GetCharacterMovement()->GravityScale = 4.0f;
	GetCharacterMovement()->Velocity = FVector(0.f, 0.f, 0.f);
}

// 梯子を掴むときの処理 7/5伴野
void ABakemonoBakariCharacter::GraspLadder()
{
	IsLadder = true;
	GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);	// プレイヤーの動きを止める
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);			// 重力の影響を受けないようにする
}

// 梯子から離れるときの処理 7/5伴野
void ABakemonoBakariCharacter::LeaveLadder()
{
	IsLadder = false;											// 梯子を掴んでいない
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);		// 重力の影響を元に戻す
}