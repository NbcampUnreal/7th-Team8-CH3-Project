#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HDMonCharacter.generated.h"


class UWidgetComponent;
class GameplayStatics;
class UHDTask_Attack;

UCLASS()
class CH03_SHOOTERGAME_API AHDMonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHDMonCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster")
	float CurrentHP;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Monster")
	float MaxHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float MoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float Atk;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	float Def;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster")
	int PointValue;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* TakeDamageMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* SkillReadyMontage;
	
	
	FTimerHandle HideOverheadTakeDamageHUDHandle;

	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitCheck();
	
	virtual void BeginPlay() override;
	virtual void OnDeath();
	

protected:
	FTimerHandle HitRecoverTimerHandle;
	FTimerHandle SkillTimerHandle;

	virtual void RecoverFromHit();
};
