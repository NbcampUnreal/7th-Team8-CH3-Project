#include "HDMonController.h"



AHDMonController::AHDMonController()
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


