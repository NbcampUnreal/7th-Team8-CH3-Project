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

		GetWorldTimerManager().SetTimer(
			DashCooldownTimerHandle,
			this,
			&AHDPlayerCharacter::ResetDash,
			DashCooldown,
			false);
		// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Dash Cooldown Started (3s)"));
	}
}

void AHDPlayerCharacter::ResetDash()
{
	bCanDash = true;
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Dash Ready!"));
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
		
		// bCanAttack false로 초기화
		ResetAttack();
	}
}

void AHDPlayerCharacter::Attack(const FInputActionValue& value)
{
	if (!Controller) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Attack Function Called!"));
	}
	
	// 플레이어가 공격 중인지 조건 확인
	if (!bCanAttack)
	{
		// 플레이어 공격 몽타주 실행
		if (AttackMontage)
		{
			PlayAnimMontage(AttackMontage);
		}
		
		// 기본 공격 난사를 막기 위해 타이머를 통해 기본공격에 쿨타임 적용, 쿨타임은 0.3초
		GetWorldTimerManager().SetTimer(
			AttackCooldownTimerHandle,
			this,
			&AHDPlayerCharacter::Fire,
			AttackCooldown,
			false
			);
		
		bCanAttack = true;
	}
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

float AHDPlayerCharacter::GetDashCooldownPercent() const // 대쉬 타이머 핸들을 이용해 대쉬의 현재 쿨타임을 반환하는 함수
{
	if (!GetWorldTimerManager().IsTimerActive(DashCooldownTimerHandle))
		return 0.0f;

	float RemainingTime = GetWorldTimerManager().GetTimerElapsed(DashCooldownTimerHandle);
	return RemainingTime / DashCooldown;
}

float AHDPlayerCharacter::GetAttackCooldownPercent() const // 공격 타이머 핸들을 이용해 대쉬의 현재 쿨타임을 반환하는 함수
{
	if (!GetWorldTimerManager().IsTimerActive(AttackCooldownTimerHandle))
		return 0.0f;
	
	float RemainingTime = GetWorldTimerManager().GetTimerElapsed(AttackCooldownTimerHandle);
	return RemainingTime / AttackCooldown;
}
