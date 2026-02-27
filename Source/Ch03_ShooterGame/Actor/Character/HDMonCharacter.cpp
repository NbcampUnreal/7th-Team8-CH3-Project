#include "HDMonCharacter.h"

#include "BrainComponent.h"
#include "HDMonController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "Components/TextBlock.h"
#include "Core/HDGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Engine/World.h"



AHDMonCharacter::AHDMonCharacter():
OverheadWidget(nullptr),
OverheadTakeDamageWidget(nullptr)
{
    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::World);
    
    OverheadTakeDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TakeDamageWidget"));
    OverheadTakeDamageWidget->SetupAttachment(GetMesh());
    OverheadTakeDamageWidget->SetWidgetSpace(EWidgetSpace::World);
    
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

    MonMoveSpeed = 150.0f;
    MonMaxHP = 60.f;
    MonHP = MonMaxHP;
    MonAtk = 20.f;
    PointValue = 100;
    GetCharacterMovement()->MaxWalkSpeed = MonMoveSpeed;
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

	MonHP = FMath::Clamp(MonHP - ActualDamage, 0.0f, MonMaxHP);
    UpdateOverheadHP();
    UpdateOverheadTakeDamage(ActualDamage);
	UE_LOG(LogTemp, Warning, TEXT("Hit damage: %f / %f"), MonHP, MonMaxHP);
	
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


    UUserWidget* OverheadWidgetInstacne = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstacne) return;

    if (UProgressBar* MonsterOverheadHPBar = Cast<UProgressBar>(OverheadWidgetInstacne->GetWidgetFromName("OverheadHP")))
    {
        float Precent = (float)MonHP / MonMaxHP;
        MonsterOverheadHPBar->SetPercent(Precent);
    }
}

void AHDMonCharacter::UpdateOverheadTakeDamage(float DamageAmount)
{
    if (!OverheadTakeDamageWidget) return;
    
    UUserWidget* OverheadTakeDamageWidgetInstance = OverheadTakeDamageWidget->GetUserWidgetObject();
    if (!OverheadTakeDamageWidgetInstance) return;
    
    if (UTextBlock* OverheadTakeDamageText = Cast<UTextBlock>(OverheadTakeDamageWidgetInstance->GetWidgetFromName("OverheadTakeDamageText")))
    {
        OverheadTakeDamageText->SetVisibility(ESlateVisibility::Visible);
        
        OverheadTakeDamageText->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), DamageAmount)));
        
        GetWorldTimerManager().SetTimer(
        HideOverheadTakeDamageHUDHandle,
        this,
        &AHDMonCharacter::HideOverheadTakeDamage,
        1,
        false
        );
    }
}

void AHDMonCharacter::HideOverheadTakeDamage()
{
    if (!OverheadTakeDamageWidget) return;
    
    UUserWidget* OverheadTakeDamageWidgetInstance = OverheadTakeDamageWidget->GetUserWidgetObject();
    if (!OverheadTakeDamageWidgetInstance) return;
    
    if (UTextBlock* OverheadTakeDamageText = Cast<UTextBlock>(OverheadTakeDamageWidgetInstance->GetWidgetFromName("OverheadTakeDamageText")))
    {
        OverheadTakeDamageText->SetVisibility(ESlateVisibility::Hidden);
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


