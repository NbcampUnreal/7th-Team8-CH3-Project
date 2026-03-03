
#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/HDMonCharacter.h"
#include "HDBossMonster.generated.h"


class UWidgetComponent;

UCLASS()
class CH03_SHOOTERGAME_API AHDBossMonster : public AHDMonCharacter
{
	public:
	
	bool HPChecked;
	
	
	AHDBossMonster();
	
	GENERATED_BODY()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	//UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual void AttackHitCheck() override;
    
	virtual void BeginPlay() override;
	
	virtual void OnDeath()  override; 

	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void Skill() override;
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void WaitSkill() override;
protected:
	FTimerHandle HitRecoverTimerHandle;

	
	virtual void RecoverFromHit()  override; 
};
