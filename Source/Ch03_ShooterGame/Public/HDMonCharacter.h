// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HDMonCharacter.generated.h"

class GameplayStatics;
UCLASS()
class CH03_SHOOTERGAME_API AHDMonCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AHDMonCharacter();
	
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float MonHP;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Monster")
	float MonMaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonMoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float MonAtk;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* TakeDamageMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* DeathMontage;

public:	
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	void AttackHitCheck();
	void OnDeath();
};
