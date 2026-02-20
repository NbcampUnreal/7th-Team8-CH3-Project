#include "HDGameInstance.h"

UHDGameInstance::UHDGameInstance()
{
	TotalScore = 0;
	CurrentStageIndex = 0;
}

void UHDGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}
