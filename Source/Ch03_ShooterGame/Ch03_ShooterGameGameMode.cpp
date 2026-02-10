// Copyright Epic Games, Inc. All Rights Reserved.

#include "Ch03_ShooterGameGameMode.h"
#include "Ch03_ShooterGamePlayerController.h"
#include "Ch03_ShooterGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACh03_ShooterGameGameMode::ACh03_ShooterGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACh03_ShooterGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}