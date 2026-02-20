#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// 디폴트값 설정
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

//함수 기능을 입력에 바인딩하기 위해 호출
void AHDPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 'movement' 바인딩을 구성합니다.
    PlayerInputComponent->BindAxis("MoveForward", this, &AHDPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AHDPlayerCharacter::MoveRight);

    // 'look' 바인딩을 구성합니다.
    PlayerInputComponent->BindAxis("Turn", this, &AHDPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &AHDPlayerCharacter::AddControllerPitchInput);

    // 'action' 바인딩을 구성합니다.
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AHDPlayerCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &AHDPlayerCharacter::StopJump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHDPlayerCharacter::Fire);
}

void AHDPlayerCharacter::MoveForward(float Value)
{
    // 어디가 '앞'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AHDPlayerCharacter::MoveRight(float Value)
{
    // 어디가 '오른쪽'인지 찾고, 플레이어가 해당 방향으로 이동하고자 한다는 것을 기록합니다.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AHDPlayerCharacter::StartJump()
{
    bPressedJump = true;
}

void AHDPlayerCharacter::StopJump()
{
    bPressedJump = false;
}

void AHDPlayerCharacter::Fire()
{

    // 발사체 발사를 시도합니다.
    if (ProjectileClass)
    {
        // 카메라 트랜스폼을 구합니다.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset이 카메라 살짝 앞에서 발사체를 스폰하도록 설정합니다.
        MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

        // MuzzleOffset을 카메라 스페이스에서 월드 스페이스로 변환합니다.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        // 조준이 살짝 위를 향하도록 왜곡합니다.
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 10.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            // 총구에 발사체를 스폰합니다.
            AHDBowProjectile* Projectile = World->SpawnActor<AHDBowProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 발사체의 초기 탄도를 설정합니다.
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->FireInDirection(LaunchDirection);
            }
        }
    }
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