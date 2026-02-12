// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameMode.h"
#include "HDPlayerCharacter.h"
AHDGameMode::AHDGameMode()
{
	DefaultPawnClass = AHDPlayerCharacter::StaticClass();
}