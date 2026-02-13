
#include "HDGameStateBase.h"
 AHDGameStateBase::AHDGameStateBase()
{
	 Score = 0;
}

int32 AHDGameStateBase::GetScore() const
{
	return Score;
}

void AHDGameStateBase::AddScore(int32 Amount)
{
	Score += Amount;
}