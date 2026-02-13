#include "HDGameStateBase.h"
#include "HDPlayerCharacter.h"
#include "HDPlayerController.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

 AHDGameStateBase::AHDGameStateBase()
{
	 Score = 0;
	 CurrentStageIndex = 0;
}

int32 AHDGameStateBase::GetScore() const
{
	return Score;
}

void AHDGameStateBase::AddScore(int32 Amount)
{
	Score += Amount;
}

void AHDGameStateBase::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = HDPlayerController->GetHUDWidget())
			{
				if (UProgressBar* PlayerHPProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterHPBar"))))
				{
					if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						float Precent = (float)HDPlayerCharacter->HP / HDPlayerCharacter->MaxHP;
						PlayerHPProgressBar->SetPercent(Precent);
					}
				}

				if (UProgressBar* PlayerManaProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterManaBar"))))
				{
					if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
					{
						float Precent = (float)HDPlayerCharacter->Mana / HDPlayerCharacter->MaxMana;
						PlayerManaProgressBar->SetPercent(Precent);
					}
				}

				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(StageTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						AHDGameStateBase* HDGameStateBase = Cast<AHDGameStateBase>(GameInstance);
						if (HDGameStateBase)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), HDGameStateBase->Score)));
						}
					}
				}

				if (UTextBlock* StageIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StageText"))))
				{
					StageIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d Stage"), CurrentStageIndex + 1)));
				}
			}
		}
	}
}
