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
}

void AHDPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(
				PlayerController->MoveAction,
				ETriggerEvent::Triggered,
				this,
				&AHDPlayerCharacter::Move
			);
		}
		if (AHDPlayerController* PlayerController = Cast<AHDPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(
				PlayerController->DashAction,
				ETriggerEvent::Triggered,
				this,
				&AHDPlayerCharacter::Dash
			);
		}
	}
}

void AHDPlayerCharacter::Move(const FInputActionValue& value)
{

}

void AHDPlayerCharacter::Dash(const FInputActionValue& value)
{

}