// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameInstance.h"

UHDGameInstance::UHDGameInstance()
{
	TotalKillCount = 0;
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UHDGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

void UHDGameInstance::AddToKillCount(int32 Amount)
{
	TotalKillCount += Amount;
}