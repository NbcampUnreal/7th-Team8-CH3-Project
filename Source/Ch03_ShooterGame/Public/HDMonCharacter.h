#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "HDMonCharacter.generated.h"

class GameplayStatics;
UCLASS()
class CH03_SHOOTERGAME_API AHDMonCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AHDMonCharacter();
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")  // 이건 몬스터 헤더로 옮겨야함
	UWidgetComponent* OverheadWidget;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	UFUNCTION(BlueprintCallable)
	void AttackHitCheck();
	void OnDeath();
	void UpdateOverheadHP();
};
