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

	UInputMappingContext* InputMappingContext;
	UInputAction* MoveAction;
	UInputAction* DashAction;
};
