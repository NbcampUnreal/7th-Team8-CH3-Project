#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	int HP;
	int MaxHP;
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
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BowStaticMesh;

	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;
	void ResetDash();
	void InitializationWeaponMesh();

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<AHDBowProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AActor> MineClass;

	bool bCanUseMine = true; 
	FTimerHandle MineCooldownTimerHandle;

	void ResetMineCooldown(); 

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UseMineItem();

	UPROPERTY(EditAnywhere, Category = "Combat")
	float MineCooldownTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxMP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float CurrentMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MPRegenRate; // 초당 회복량

	// MP 사용 가능 여부 확인 함수
	bool CanUseMP(float Cost);
	// MP 소모 함수
	void UseMP(float Cost);




	UFUNCTION()
	void Fire();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};