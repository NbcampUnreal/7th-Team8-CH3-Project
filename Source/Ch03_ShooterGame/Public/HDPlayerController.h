#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH03_SHOOTERGAME_API AHDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHDPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DashAction;

protected:
	virtual void BeginPlay() override;
};
