#include "HDGameStateBase.h"
#include "HDPlayerCharacter.h"
#include "HDGameInstance.h"
#include "HDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

 AHDGameStateBase::AHDGameStateBase()
{
	 Score = 0;
	 CurrentStageIndex = 0;
	 LevelDuration = 10.0f;
	 MaxStages = 2;
}

int32 AHDGameStateBase::GetScore() const
{
	return Score;
}

void AHDGameStateBase::AddScore(int32 Amount)
{
	Score += Amount;
}

void AHDGameStateBase::StartStage()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController Found"));

		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			UE_LOG(LogTemp, Warning, TEXT("Cast Success"));
			HDPlayerController->ShowCharacterHUD();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cast Failed"));
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			HDPlayerController->ShowCharacterHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);

		if (HDGameInstance)
		{
			CurrentStageIndex = HDGameInstance->CurrentStageIndex;
		}
	}

	GetWorldTimerManager().SetTimer(
		StageTimerHandle,
		this,
		&AHDGameStateBase::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void AHDGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHDGameStateBase::UpdateHUD,
		0.1f,
		true
	);
}

void AHDGameStateBase::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = HDPlayerController->GetHUDWidget())
			{
				if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
				{
					if (UProgressBar* PlayerHPProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterHPBar"))))
					{
						float Precent = (float)HDPlayerCharacter->HP / HDPlayerCharacter->MaxHP;
						PlayerHPProgressBar->SetPercent(Precent);
					}

					if (UProgressBar* PlayerManaProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("CharacterManaBar"))))
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
					ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), Score)));
				}

				if (UTextBlock* StageIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StageText"))))
				{
					StageIndexText->SetText(FText::FromString(FString::Printf(TEXT("%d Stage"), CurrentStageIndex + 1)));
				}
			}
		}
	}
}

void AHDGameStateBase::OnLevelTimeUp()
{
	EndStage();
}

void AHDGameStateBase::EndStage()
{
	UE_LOG(LogTemp, Warning, TEXT("StageMapNames Num: %d"), StageMapNames.Num());
	UE_LOG(LogTemp, Warning, TEXT("CurrentStageIndex: %d"), CurrentStageIndex);

	GetWorldTimerManager().ClearTimer(StageTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);

		if (HDGameInstance)
		{
			//AddScore(Score);
			CurrentStageIndex++;
			HDGameInstance->CurrentStageIndex = CurrentStageIndex;
		}
	}

	if (CurrentStageIndex >= MaxStages)
	{
		OnGameOver();
		return;
	}

	if (StageMapNames.IsValidIndex(CurrentStageIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), StageMapNames[CurrentStageIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AHDGameStateBase::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			HDPlayerController->SetPause(true);
			HDPlayerController->ShowMainMenu(true);
		}
	}
}