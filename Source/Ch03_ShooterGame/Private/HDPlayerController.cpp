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

	}
}