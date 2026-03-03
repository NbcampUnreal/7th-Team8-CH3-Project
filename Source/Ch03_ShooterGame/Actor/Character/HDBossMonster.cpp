// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Character/HDBossMonster.h"
#include "Actor/Character/HDMonCharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
		AnimInstance->Montage_Play(SkillMontage,2.0f);
	}
	
	
	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector StartLoc = GetActorLocation() + (GetActorForwardVector() * CapsuleRadius);
	FVector EndLoc = StartLoc + (GetActorForwardVector() * 300.0f);
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
    
	bool bResult = GetWorld()->SweepSingleByChannel(
		 HitResult,
		 StartLoc, 
		 EndLoc,  
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
	UE_LOG(LogTemp, Error, TEXT("나는 보스다! 넉백 코드가 없다!"));
	
	if (CurrentHP <= 0.0f)
	{
		OnDeath();
		return ActualDamage;
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
			0.3f, 
			false
		);
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
    
	float AttackRadius = 200.0f;

	DrawDebugSphere(GetWorld(), EndLoc, AttackRadius, 16, FColor::Green, false, 2.0f);
    
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLoc, 
		EndLoc,  
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius), // 여기도 하드코딩된 200.0f 대신 변수 사용
		Params
	  );
    
	if (bResult)
	{
		if (AActor* Target = HitResult.GetActor())
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