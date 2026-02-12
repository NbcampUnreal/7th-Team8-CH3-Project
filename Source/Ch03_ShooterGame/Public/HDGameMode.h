// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HDGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API AHDGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void StartPlay() override;
};


