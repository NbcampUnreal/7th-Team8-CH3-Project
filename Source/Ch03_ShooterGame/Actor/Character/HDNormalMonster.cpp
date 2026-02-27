// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/HDNormalMonster.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHDNormalMonster::AHDNormalMonster()
{
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::World);
}
void AHDNormalMonster::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
	MoveSpeed = 150.0f;
	MaxHP = 60.f;
	CurrentHP = MaxHP;
	Atk = 20.f;
	Def = 4.0f;
	PointValue = 100;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}


float AHDNormalMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	UpdateOverheadHP();	
	if (DamageCauser && CurrentHP > 0.0f) 
	{
		FVector PushDirection = GetActorLocation() - DamageCauser->GetActorLocation();
		PushDirection.Z = 0.0f;
		PushDirection.Normalize();
	
		LaunchCharacter(PushDirection * 1000.0f, true, false);
	}
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
			1.0f, 
			false
		);
	}
	return ActualDamage;
}

void AHDNormalMonster::AttackHitCheck()
{
	Super::AttackHitCheck();
	
	
	
}

void AHDNormalMonster::OnDeath()
{
	Super::OnDeath();
}

void AHDNormalMonster ::UpdateOverheadHP() const
{
	if (!OverheadWidget) return;

	const UUserWidget* OverheadWidgetInstacne = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstacne) return;

	if (UProgressBar* MonsterOverheadHPBar = Cast<UProgressBar>(OverheadWidgetInstacne->GetWidgetFromName("OverheadHP")))
	{
		const float Precent = static_cast<float>(CurrentHP) / MaxHP;
		MonsterOverheadHPBar->SetPercent(Precent);
	}
}

void AHDNormalMonster::RecoverFromHit()
{
	Super::RecoverFromHit();

}
