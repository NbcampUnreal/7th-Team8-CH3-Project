// Fill out your copyright notice in the Description page of Project Settings.


#include "HDMonController.h"
#include "NavigationSystem.h"

AHDMonController::AHDMonController()
{
	RepeatInterval = 3.0f;
}

void AHDMonController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(
		RepeatTimerHandle,
		this,
		&AHDMonController::OnRepeatTimer,
		RepeatInterval,
		true
	);
}

void AHDMonController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AHDMonController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (!IsValid(CurrentPawn))
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem)return;

	FNavLocation NextLocation;

	if (NavSystem->GetRandomReachablePointInRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		MoveToLocation(NextLocation.Location);
	}
}
