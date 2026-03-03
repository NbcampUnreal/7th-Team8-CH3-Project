#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h"
#include "HDPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class AHDBowProjectile;
class UAnimMontage;

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
	UAnimMontage* DashMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* TakeDamageMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRolling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* DashSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	class UAudioComponent* FootstepAudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* FootstepSound;

	UFUNCTION(BlueprintCallable, Category = "Animation")

	float GetMovementDirection() const;
	
	int HP;
	int MaxHP;
	float Mana;
	float MaxMana;
	float Atk;
	float DashCooldown;
	float AttackCooldown;
	
	float GetDashCooldownPercent() const;
	float GetAttackCooldownPercent() const;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	
	UFUNCTION()
	void Dash(const FInputActionValue& value);
	
	UFUNCTION()
	void Attack(const FInputActionValue& value);
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BowStaticMesh;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	void OnDeath();
	void ResetDash();
	void ResetAttack();
	void InitializationWeaponMesh();
	
	bool bCanDash = true;
	bool bCanAttack = false;
	FTimerHandle DashCooldownTimerHandle;
	FTimerHandle AttackCooldownTimerHandle;
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};