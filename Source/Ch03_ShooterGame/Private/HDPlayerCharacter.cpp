#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "HDBowProjectile.h"
#include "HDMonCharacter.h"
#include "HDBowHUD.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <Kismet/GameplayStatics.h>

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

	MaxHP = 100.0f;
	HP = MaxHP;

	MaxMana = 100.0f;
	Mana = MaxMana;
}

void AHDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(
				PlayerController->MoveAction,ETriggerEvent::Triggered,this,&AHDPlayerCharacter::Move);
			EnhancedInput->BindAction(
				PlayerController->AttackAction, ETriggerEvent::Started, this, &AHDPlayerCharacter::Attack);
		}

		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(PlayerController->DashAction,ETriggerEvent::Triggered,this,&AHDPlayerCharacter::Dash);
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
	if (value.Get<bool>() && bCanDash)
	{
		if (DashMontage)
		{
			PlayAnimMontage(DashMontage);
		}
		const float DashStrength = 2500.0f;
		const FVector DashDir = GetActorForwardVector();

		LaunchCharacter(DashDir * DashStrength, true, false);

		bCanDash = false;

		GetWorldTimerManager().SetTimer(DashCooldownTimerHandle,this,&AHDPlayerCharacter::ResetDash,3.0f,false);
		// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Dash Cooldown Started (3s)"));
	}
}


void AHDPlayerCharacter::ResetDash()
{
	bCanDash = true;
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Dash Ready!"));
}
void AHDPlayerCharacter::Fire()
{
	
	if (ProjectileClass)
	{
		FVector CharacterLocation = GetActorLocation();
		FRotator CharacterRotation = GetActorRotation();

		
		MuzzleOffset.Set(100.0f, 0.0f, 50.0f);		
		FVector MuzzleLocation = CharacterLocation + FTransform(CharacterRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CharacterRotation;		
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			
			AHDBowProjectile* Projectile = World->SpawnActor<AHDBowProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}
void AHDPlayerCharacter::Attack(const FInputActionValue& value)
{
	if (!Controller) return;

	bool bIsPressed = value.Get<bool>();

	if(bIsPressed)
	{
		if (AttackMontage)
		{
			PlayAnimMontage(AttackMontage);
		}
		Fire();
	}
}

float AHDPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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
	UE_LOG(LogTemp, Warning, TEXT("Hit damage: %f / %f"), HP, MaxHP);


	return ActualDamage;
}
