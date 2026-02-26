#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "HDMonCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Actor/HDBowProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Core/HDGameState.h"
#include "GameFramework/CharacterMovementComponent.h"

AHDPlayerCharacter::AHDPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1000.0f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;

		GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f);
	}

	InitializationWeaponMesh();

	MaxHP = 100.0f;
	HP = MaxHP;

	MaxMana = 100.0f;
	Mana = MaxMana;

	DashCooldown = 3.0f;
	AttackCooldown = 0.3f;
}

void AHDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(
				PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AHDPlayerCharacter::Move);
			EnhancedInput->BindAction(
				PlayerController->AttackAction, ETriggerEvent::Started, this, &AHDPlayerCharacter::Attack);
		}

		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(PlayerController->DashAction, ETriggerEvent::Triggered, this,
			                          &AHDPlayerCharacter::Dash);
		}
	}
}

void AHDPlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(FVector::ForwardVector, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(FVector::RightVector, MoveInput.Y);
	}
}

void AHDPlayerCharacter::Dash(const FInputActionValue& value)
{
	if (!value.Get<bool>() || !bCanDash || bCanAttack) return;

	bIsRolling = true;

	FVector DashDir = GetLastMovementInputVector();
	if (DashDir.IsNearlyZero()) DashDir = GetActorForwardVector();
	DashDir.Normalize();

	SetActorRotation(DashDir.Rotation());

	if (DashMontage) PlayAnimMontage(DashMontage);

	LaunchCharacter(DashDir * 2500.0f, true, true);

	bCanDash = false;
	bCanAttack = true;

	GetWorldTimerManager().SetTimer(DashCooldownTimerHandle, this, &AHDPlayerCharacter::ResetDash, DashCooldown, false);

	FTimerHandle RollRestoreHandle;
	GetWorldTimerManager().SetTimer(RollRestoreHandle, FTimerDelegate::CreateLambda([this]()
		{
			bIsRolling = false;
			bCanAttack = false;
		}), 0.6f, false);
}

void AHDPlayerCharacter::ResetDash()
{
	bCanDash = true;
}

void AHDPlayerCharacter::ResetAttack()
{
	bCanAttack = false;
}

void AHDPlayerCharacter::Fire()
{
	if (ProjectileClass)
	{
		FVector CharacterLocation = GetActorLocation();
		FRotator CharacterRotation = GetActorRotation();
		
		MuzzleOffset.Set(150.0f, 0.0f, 00.0f);		
		FVector MuzzleLocation = CharacterLocation + FTransform(CharacterRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CharacterRotation;		
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();

		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AHDBowProjectile* Projectile = World->SpawnActor<AHDBowProjectile>(
				ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
		
		ResetAttack();
	}
}

void AHDPlayerCharacter::Attack(const FInputActionValue& value)
{
	if (!Controller) return;

	if (bCanAttack) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Attack Function Called!"));
	}
	
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
	
	bCanAttack = true;

	GetWorldTimerManager().SetTimer(
		AttackCooldownTimerHandle,
		this,
		&AHDPlayerCharacter::Fire,
		AttackCooldown,
		false
	);
}

float AHDPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                     AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && TakeDamageMontage)
	{
		AnimInstance->Montage_Play(TakeDamageMontage);
	}

	HP = FMath::Clamp(HP - ActualDamage, 0.0f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Hit damage: %d / %d"), HP, MaxHP);
	
	if (HP <= 0)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

void AHDPlayerCharacter::OnDeath() // 플레이어 죽었을 때 호출되는 함수
{
	AHDGameState* GameState = Cast<AHDGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->OnGameOver();
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		// DeathMontage를 실행시켜주는 함수
		AnimInstance->Montage_Play(DeathMontage);
		// 캐릭터의 캡슐 콜리전을 끈다
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 캐릭터 매쉬의 충돌을 끈다
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AHDPlayerCharacter::InitializationWeaponMesh()
{
	BowStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bow"));
	BowStaticMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Bow(
		TEXT("/Script/Engine.StaticMesh'/Game/Fab/Survival_Kit_-_Bow/survival_kit_bow.survival_kit_bow'"));

	if (Bow.Succeeded())
	{
		BowStaticMesh->SetStaticMesh(Bow.Object);
	}

	BowStaticMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow_Socket"));
}

float AHDPlayerCharacter::GetDashCooldownPercent() const
{
	if (!GetWorldTimerManager().IsTimerActive(DashCooldownTimerHandle))
		return 0.0f;

	float RemainingTime = GetWorldTimerManager().GetTimerElapsed(DashCooldownTimerHandle);
	return RemainingTime / DashCooldown;
}

float AHDPlayerCharacter::GetAttackCooldownPercent() const
{
	if (!GetWorldTimerManager().IsTimerActive(AttackCooldownTimerHandle))
		return 0.0f;
	
	float RemainingTime = GetWorldTimerManager().GetTimerElapsed(AttackCooldownTimerHandle);
	return RemainingTime / AttackCooldown;
}

