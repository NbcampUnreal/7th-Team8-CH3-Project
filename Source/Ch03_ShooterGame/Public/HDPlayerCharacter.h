#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HDPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class AHDBowProjectile;

UCLASS()

class CH03_SHOOTERGAME_API AHDPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHDPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* DashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UAnimMontage* AttackMontage;

	float HP;
	float MaxHP;
	float Mana;
	float MaxMana;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Dash(const FInputActionValue& value);
	UFUNCTION()
	void Attack(const FInputActionValue& value);
	UPROPERTY(EditDefaultsOnly, Category = "AI Animation")
	UAnimMontage* TakeDamageMontage;
	
	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;
	void ResetDash();

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AHDBowProjectile> ProjectileClass;

	// 발사체를 발사하는 함수입니다.
	UFUNCTION()
	void Fire();

	// 카메라 위치로부터의 총구 오프셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};