#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HDMonController.generated.h"

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class CH03_SHOOTERGAME_API AHDMonController : public AAIController
{
	GENERATED_BODY()
	
public:
	AHDMonController();
	
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const { return HUDWidgetInstance; }

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

};
