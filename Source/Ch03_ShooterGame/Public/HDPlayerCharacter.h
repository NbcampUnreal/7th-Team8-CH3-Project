#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h" // .generated.h 파일명은 반드시 클래스 파일명과 일치해야 합니다.
#include "HDBowProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HDPlayerCharacter.generated.h"



UCLASS()
 
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

class CH03_SHOOTERGAME_API AHDPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // 이 캐릭터의 프로퍼티에 적용되는 디폴트값 설정
    AHDPlayerCharacter();

	float HP;
	float MaxHP;
	float Mana;
	float MaxMana;
  
    // 게임 시작 또는 스폰 시 호출
    virtual void BeginPlay() override;

    // 스폰할 화살(발사체) 클래스입니다.
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AHDBowProjectile> ProjectileClass;

public:
    // 프레임마다 호출
    virtual void Tick(float DeltaTime) override;


protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Dash(const FInputActionValue& value);
	bool bCanDash = true;
	FTimerHandle DashCooldownTimerHandle;
	void ResetDash();
  
     // 화살을 발사하는 함수입니다.
    UFUNCTION()
    void Fire();

    
};