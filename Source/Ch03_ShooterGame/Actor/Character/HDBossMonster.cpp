// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/HDBossMonster.h"
#include "Actor/Character/HDMonCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AHDBossMonster::AHDBossMonster()
{
	
}
void AHDBossMonster::BeginPlay()
{
	Super::BeginPlay();
	MoveSpeed = 200.0f;
	MaxHP = 300.f;
	CurrentHP = MaxHP;
	Atk = 40.f;
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
		AnimInstance->Montage_Play(SkillMontage);
	}
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->SweepSingleByChannel(
	   HitResult,
	   GetActorLocation(),
	   GetActorLocation() + GetActorForwardVector() * 300.0f, 
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
				Atk * 2,      // 헤더에 선언한 공격력 변수
				GetController(),   // 가해자(몬스터)의 컨트롤러
				this,              // 가해자(몬스터) 자신
				UDamageType::StaticClass()
			);
		}
	}
}

void AHDBossMonster::WaitSkill()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && SkillReadyMontage)
	{
		AnimInstance->Montage_Play(SkillReadyMontage);
	}
	GetWorld()->GetTimerManager().SetTimer(
		SkillTimerHandle, 
		this, 
		&AHDBossMonster::Skill, 
		2.0f, 
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
			&AHDBossMonster::RecoverFromHit,
			1.0f, 
			false
		);
	}
	return ActualDamage;
}

void AHDBossMonster::AttackHitCheck()
{
	Super::AttackHitCheck();
}