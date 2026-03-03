
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
    

	virtual void AttackHitCheck()  override; 
    
	virtual void BeginPlay() override;
    
	virtual void OnDeath()  override; 

	void Skill(); 
	void WaitSkill(); 
protected:
	FTimerHandle HitRecoverTimerHandle;


	virtual void RecoverFromHit()  override; 
};
