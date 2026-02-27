#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HDMonCharacter.generated.h"


class GameplayStatics;
class UHDTask_Attack;
UCLASS()
class CH03_SHOOTERGAME_API AHDMonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHDMonCharacter();
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  // 이건 몬스터 헤더로 옮겨야함
	UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  // 이건 몬스터 헤더로 옮겨야함
	UWidgetComponent* OverheadTakeDamageWidget;
	
	FTimerHandle HideOverheadTakeDamageHUDHandle;
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	virtual void AttackHitCheck();
	
	virtual void BeginPlay() override;
	void OnDeath();
	void UpdateOverheadHP();
	void UpdateOverheadTakeDamage(float DamageAmount);
	void HideOverheadTakeDamage();

protected:
	FTimerHandle HitRecoverTimerHandle;
	FTimerHandle SkillTimerHandle;

	void RecoverFromHit();
};
