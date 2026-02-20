// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameMode.h"
#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "HDGameStateBase.h"

AHDGameMode::AHDGameMode()
{
	DefaultPawnClass = AHDPlayerCharacter::StaticClass();
	PlayerControllerClass = AHDPlayerController::StaticClass();
	GameStateClass = AHDGameStateBase::StaticClass();
}