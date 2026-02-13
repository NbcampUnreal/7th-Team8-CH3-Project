#include "HDPlayerController.h"
#include "EnhancedInputSubsystems.h"

AHDPlayerController::AHDPlayerController()
	:InputMappingContext(nullptr),
	 MoveAction(nullptr),
	 DashAction(nullptr)
{

}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}