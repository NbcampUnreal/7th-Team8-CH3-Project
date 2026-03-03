// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/HDNormalMonster.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AHDNormalMonster::AHDNormalMonster():
OverheadWidget(nullptr),
OverheadTakeDamageWidget(nullptr)
{
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::World);
    
	OverheadTakeDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("TakeDamageWidget"));
	OverheadTakeDamageWidget->SetupAttachment(GetMesh());
	OverheadTakeDamageWidget->SetWidgetSpace(EWidgetSpace::World);
}
void AHDNormalMonster::BeginPlay()
{
	Super::BeginPlay();
	MoveSpeed = 150.0f;
	MaxHP = 100.f;
	CurrentHP = MaxHP;
	Atk = 20.f;
	Def = 4.0f;
	PointValue = 100;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	UpdateOverheadHP();
}


float AHDNormalMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if (DamageCauser && CurrentHP > 0.0f) 
	{
		FVector PushDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		PushDirection.Z = 0.0f;
		PushDirection.Normalize();
	
		LaunchCharacter(PushDirection * 1000.0f, true, false);
	}
	if (CurrentHP <= 0.0f)
	{
		OnDeath();
		return ActualDamage;
	}
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (TakeDamageMontage) AnimInstance->Montage_Play(TakeDamageMontage,1.2f);
	}
	
	UpdateOverheadHP();
	UpdateOverheadTakeDamage(ActualDamage);
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
      
		AIController->StopMovement();

		if (AIController->GetBrainComponent())
		{
			AIController->GetBrainComponent()->PauseLogic("HitStun");
		}
		if (HitRecoverTimerHandle.IsValid())
		{
			AIController->GetWorldTimerManager().ClearTimer(HitRecoverTimerHandle);
		}
     
		GetWorld()->GetTimerManager().SetTimer(
			HitRecoverTimerHandle,
			this,
			&AHDNormalMonster::RecoverFromHit,
			0.6f, 
			false
		);
	}
	return ActualDamage;
}

void AHDNormalMonster::AttackHitCheck()
{
	Super::AttackHitCheck();
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
				Atk, 
				GetController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}
}

void AHDNormalMonster::OnDeath()
{
	Super::OnDeath();
}

void AHDNormalMonster::UpdateOverheadHP() const
{
	if (!OverheadWidget) return;

	const UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance) return;

	if (UProgressBar* MonsterOverheadHPBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName("OverheadHP")))
	{
		const float Precent = static_cast<float>(CurrentHP) / MaxHP;
		MonsterOverheadHPBar->SetPercent(Precent);
	}
}

void AHDNormalMonster::UpdateOverheadTakeDamage(float DamageAmount)
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
		&AHDNormalMonster::HideOverheadTakeDamage,
		1,
		false
		);
	}
}

void AHDNormalMonster::HideOverheadTakeDamage() const
{
	if (!OverheadTakeDamageWidget) return;
    
	UUserWidget* OverheadTakeDamageWidgetInstance = OverheadTakeDamageWidget->GetUserWidgetObject();
	if (!OverheadTakeDamageWidgetInstance) return;
    
	if (UTextBlock* OverheadTakeDamageText = Cast<UTextBlock>(OverheadTakeDamageWidgetInstance->GetWidgetFromName("OverheadTakeDamageText")))
	{
		OverheadTakeDamageText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AHDNormalMonster::RecoverFromHit()
{
	Super::RecoverFromHit();
}


