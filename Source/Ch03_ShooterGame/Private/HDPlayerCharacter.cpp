#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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