#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HDCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CH03_SHOOTERGAME_API AHDCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AHDCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
