// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/HDMonCharacter.h"
#include "HDNormalMonster.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API AHDNormalMonster : public AHDMonCharacter
{
	GENERATED_BODY()
public:
	AHDNormalMonster();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    
	
	virtual void AttackHitCheck() override; // 가상 함수로 변경
    
	virtual void BeginPlay() override;
    
	virtual void OnDeath() override; // 가상 함수로 변경
	void UpdateOverheadHP() const; // 가상 함수로 변경

	
protected:
	FTimerHandle HitRecoverTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  // 이건 몬스터 헤더로 옮겨야함
	UWidgetComponent* OverheadWidget;
	
	virtual void RecoverFromHit() override; // 가상 함수로 변경
};
