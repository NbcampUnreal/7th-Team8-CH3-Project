#include "HDMonController.h"

#include "Blueprint/UserWidget.h"


AHDMonController::AHDMonController():
HUDWidgetClass(nullptr),
HUDWidgetInstance(nullptr)
{
}

void AHDMonController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}