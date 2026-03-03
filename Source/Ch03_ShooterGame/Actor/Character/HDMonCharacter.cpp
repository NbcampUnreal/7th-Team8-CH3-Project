#include "HDMonCharacter.h"
#include "BrainComponent.h"
#include "HDMonController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "HDPlayerCharacter.h"
#include "Core/HDGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Engine/World.h"



AHDMonCharacter::AHDMonCharacter()
{

    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    AIControllerClass = AHDMonController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
 
}


void AHDMonCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->bEnablePhysicsInteraction = false;
}

float AHDMonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f)
	{
		return 0.0f;
	}

	CurrentHP = FMath::Clamp(CurrentHP - ActualDamage, 0.0f, MaxHP);
	UE_LOG(LogTemp, Warning, TEXT("Hit damage: %f / %f"), CurrentHP, MaxHP);
	
    if (CurrentHP <= 0.0f)
	{
		OnDeath();
		return ActualDamage;
	}
	
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
    if (AnimInstance && TakeDamageMontage)
	{
		AnimInstance->Montage_Play(TakeDamageMontage);
	}
    
    return ActualDamage;
}

void AHDMonCharacter::OnDeath()
{
    UE_LOG(LogTemp, Warning, TEXT("Monster Died!"));

    if (UWorld* World = GetWorld())
    {
        if (AHDGameState* GameState = World->GetGameState<AHDGameState>())
        {
            GameState->AddScore(PointValue);
        }
    }

    if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && DeathMontage)
    {
        AnimInstance->Montage_Play(DeathMontage);
    }
    
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();
    
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DetachFromControllerPendingDestroy();

    SetLifeSpan(2.0f);
}



void AHDMonCharacter::AttackHitCheck()
{
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);

    bool bResult = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * 100.0f, // 사거리 100
        FQuat::Identity,
        ECollisionChannel::ECC_Pawn,
        FCollisionShape::MakeSphere(50.0f),
        Params
    );

    if (bResult)
    {
        if (AActor* Target = HitResult.GetActor())
        {
            // 로그 확인
            UE_LOG(LogTemp, Warning, TEXT("Hit Target: %s"), *Target->GetName());

            UGameplayStatics::ApplyDamage(
                Target,
                Atk,      // 헤더에 선언한 공격력 변수
                GetController(),   // 가해자(몬스터)의 컨트롤러
                this,              // 가해자(몬스터) 자신
                UDamageType::StaticClass()
            );
        }
    }
}


void AHDMonCharacter::RecoverFromHit()
{
    if (CurrentHP > 0.0f)
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (AICon->GetBrainComponent())
            {
                AICon->GetBrainComponent()->ResumeLogic("HitStun");
            }
        }
    }
}


