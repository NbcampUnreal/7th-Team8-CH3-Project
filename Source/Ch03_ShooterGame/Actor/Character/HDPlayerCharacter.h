#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	
	int HP;
	int MaxHP;
	float Mana;
	float MaxMana;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 발사체를 발사하는 함수입니다.
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

	// 카메라 위치로부터의 총구 오프셋입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	void OnDeath();
	void ResetDash();
	void InitializationWeaponMesh();
	
	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;
	
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
};