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
	
	bool bHasUsed50PercentSkill = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FootstepSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* FootstepAudioComp;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundAttenuation* FootstepAttenuation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* AttackSound;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* TakeDamageMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* SkillMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* SkillReadyMontage;

	virtual bool SkillReadyIsActive();
	
	FTimerHandle HideOverheadTakeDamageHUDHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRolling = false;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitCheck();
	
	virtual void BeginPlay() override;
	virtual void OnDeath();
	
	virtual void Skill(); 
	virtual void WaitSkill(); 
	
	FTimerHandle HitRecoverTimerHandle;
	FTimerHandle SkillTimerHandle;
protected:
	

	virtual void RecoverFromHit();
};
