
#pragma once

#include "CoreMinimal.h"
#include "Actor/Character/HDMonCharacter.h"
#include "HDNormalMonster.generated.h"

class UWidgetComponent;

UCLASS()
class CH03_SHOOTERGAME_API AHDNormalMonster : public AHDMonCharacter
{
	GENERATED_BODY()
public:
	AHDNormalMonster();
	
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void AttackHitCheck() override;
	virtual void BeginPlay() override;
	virtual void OnDeath() override; 

	void UpdateOverheadHP() const;
	void UpdateOverheadTakeDamage(float DamageAmount);
	void HideOverheadTakeDamage() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  
	UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  
	UWidgetComponent* OverheadTakeDamageWidget;
protected:
	FTimerHandle HitRecoverTimerHandle;
	FTimerHandle HideOverheadTakeDamageHUDHandle;
	
	virtual void RecoverFromHit() override;
};
