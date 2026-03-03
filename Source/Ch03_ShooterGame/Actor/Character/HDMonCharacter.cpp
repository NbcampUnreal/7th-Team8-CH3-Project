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

bool AHDMonCharacter::SkillReadyIsActive()
{
    float HPPercentage = CurrentHP / MaxHP;
    
    if ( HPPercentage <= 0.3f && !bHasUsed30PercentSkill)
    {
        bHasUsed50PercentSkill = true; // 이제 썼다고 기억함
        return true;
    }
    if (HPPercentage <= 0.5f && !bHasUsed30PercentSkill)
    {
        bHasUsed30PercentSkill = true; // 이제 썼다고 기억함
        return true;
    }    
    
    return false;
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

void AHDMonCharacter::Skill()
{
   
}

void AHDMonCharacter::WaitSkill()
{
    
}


void AHDMonCharacter::AttackHitCheck()
{
   
}




void AHDMonCharacter::RecoverFromHit()
{
    if (CurrentHP <= 0.0f) return;

    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBrainComponent* Brain = AICon->GetBrainComponent())
        {
            Brain->ResumeLogic("HitStun");           
            Brain->RestartLogic(); 
        }
    }
}