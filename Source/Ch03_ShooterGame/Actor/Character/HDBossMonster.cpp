// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/HDBossMonster.h"
#include "Actor/Character/HDMonCharacter.h"
#include "Actor/Character/HDPlayerCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AHDBossMonster::AHDBossMonster()
{
	
	bHasUsed50PercentSkill = false;
}
void AHDBossMonster::BeginPlay()
{
	Super::BeginPlay();
	MoveSpeed = 300.0f;
	MaxHP = 300.f;
	CurrentHP = MaxHP;
	Atk = 40.0f;
	Def = 5.0f;
	PointValue = 1000;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	
}

void AHDBossMonster::OnDeath()
{
	Super::OnDeath();
}

void AHDBossMonster::Skill()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && SkillMontage)
	{
		AnimInstance->Montage_Play(SkillMontage, 1.0f);
	}
    
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, this);
	FVector StartLoc = GetActorLocation();
	FVector EndLoc = StartLoc + (GetActorForwardVector() * 300.0f);
	float AttackRadius = 500.0f;
	
	bool bResult = GetWorld()->SweepMultiByChannel(
		HitResults, StartLoc, EndLoc, FQuat::Identity,
		ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius), Params
	);

	if (bResult)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* Target = Hit.GetActor())
			{
				if (Target != this && Target->IsA(AHDPlayerCharacter::StaticClass()))
				{
					UGameplayStatics::ApplyDamage(
						Target,
						Atk * 2, 
						GetController(),
						this,
						nullptr);
					UE_LOG(LogTemp, Warning, TEXT("광역 스킬 히트: %s"), *Target->GetName());
				}
			}
		}
	}

	if (AAIController* AiController = Cast<AAIController>(GetController()))
	{
		if (UBrainComponent* BC = AiController->GetBrainComponent())
		{
			BC->ResumeLogic("BossSkill");
			BC->RestartLogic();
			UE_LOG(LogTemp, Warning, TEXT("스킬 완료 (AI 재개)"));
		}
	}
}
bool AHDBossMonster::SkillReadyIsActive()
{
	if (MaxHP <= 0.f) return false;
	float HPPercentage = CurrentHP / MaxHP;
	if (GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle)) return false;
	
	if (HPPercentage <= 0.5f && !bHasUsed50PercentSkill)
	{
		bHasUsed50PercentSkill = true; 
		return true;
	}    

	return false;
}

void AHDBossMonster::WaitSkill()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle))
	{
		return;
	}
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		if (AIC->BrainComponent)
		{
			AIC->BrainComponent->PauseLogic("BossSkill"); // 태그 통일
		}
		AIC->StopMovement();
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && SkillReadyMontage)
	{
		AnimInstance->Montage_Play(SkillReadyMontage);
	}
    
	UE_LOG(LogTemp, Error, TEXT("보스 스킬 대기중 (AI 일시정지)"));

	
	GetWorld()->GetTimerManager().SetTimer(
	   SkillTimerHandle, 
	   this, 
	   &AHDBossMonster::Skill, 
	   1.0f, 
	   false 
	);
}


void AHDBossMonster::RecoverFromHit()
{
	Super::RecoverFromHit();
}

float AHDBossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                 AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0.0f)
	{
		OnDeath();
		return ActualDamage;
	}
	
	if (!GetWorld()->GetTimerManager().IsTimerActive(SkillTimerHandle))
	{
		if (SkillReadyIsActive())
		{
			WaitSkill();
		}
	}
	
	
	return ActualDamage;
}

void AHDBossMonster::AttackHitCheck()
{
    
	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	UE_LOG(LogTemp, Warning, TEXT("공격중"));
	
	FVector HeightOffset = FVector(0.0f, 0.0f, -150.0f);
    
	FVector StartLoc = GetActorLocation() + (GetActorForwardVector() * CapsuleRadius) + HeightOffset;
	FVector EndLoc = StartLoc + (GetActorForwardVector() * 150.0f);
    
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
    
	float AttackRadius = 150.0f;

	DrawDebugSphere(GetWorld(), EndLoc, AttackRadius, 16, FColor::Green, false, 2.0f);


	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc,
		EndLoc,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(150.0f),
		Params
		);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (HitRecoverTimerHandle.IsValid())
		{
			AIController->GetWorldTimerManager().ClearTimer(HitRecoverTimerHandle);
		}
		if (bResult)
		{
			if (AActor* Target = HitResult.GetActor())
			{
				if (AHDPlayerCharacter* Player = Cast<AHDPlayerCharacter>(Target))
				{
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
	}
}
