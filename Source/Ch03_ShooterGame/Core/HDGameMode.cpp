// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameMode.h"
#include "HDGameStateBase.h"
#include "Actor/Character/HDPlayerCharacter.h"
#include "Actor/Character/HDPlayerController.h"

AHDGameMode::AHDGameMode()
{
	DefaultPawnClass = AHDPlayerCharacter::StaticClass();
	PlayerControllerClass = AHDPlayerController::StaticClass();
	GameStateClass = AHDGameStateBase::StaticClass();
}
