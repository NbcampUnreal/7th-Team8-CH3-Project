#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "HDMonController.generated.h"

UCLASS()
class CH03_SHOOTERGAME_API AHDMonController : public AAIController
{
	GENERATED_BODY()
	

public:
	AHDMonController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess()override;


private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
};
