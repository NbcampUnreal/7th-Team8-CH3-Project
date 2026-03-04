// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameState.h"
#include "HDGameInstance.h"
#include "Actor/Character/HDPlayerCharacter.h"
#include "Actor/Character/HDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/SpawnVolume.h"
#include "Actor/Character/HDMonCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

AHDGameState::AHDGameState()
{
	Score = 0;
	LevelDuration = 15.0f; 
	CurrentLevelIndex = 0;
	MaxLevels = 10;
}

int32 AHDGameState::GetScore() const
{
	return Score;
}

void AHDGameState::AddScore(int32 Amount)
{
	Score += Amount;

	if (UHDGameInstance* GameInstance = Cast<UHDGameInstance>(GetGameInstance()))
	{
		GameInstance->AddToScore(Amount);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void AHDGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHDGameState::UpdateHUD,
		0.1f,
		true
	);
}

void AHDGameState::UpdateHUD()
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
					
					if (UProgressBar* PlayerDashProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("Dash_CoolDown"))))
					{
						float Cooldown = HDPlayerCharacter->GetDashCooldownPercent();
						if (Cooldown > 0.0f)
						{
							PlayerDashProgressBar->SetVisibility(ESlateVisibility::Visible);
							PlayerDashProgressBar->SetPercent(1.0 - HDPlayerCharacter->GetDashCooldownPercent());
						}
						else
						{
							PlayerDashProgressBar->SetVisibility(ESlateVisibility::Hidden);
						}
					}
					
					if (UProgressBar* PlayerAttackProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("Attack_CoolDown"))))
					{
						float Cooldown = HDPlayerCharacter->GetAttackCooldownPercent();
						if (Cooldown > 0.0f)
						{
							PlayerAttackProgressBar->SetVisibility(ESlateVisibility::Visible);
							PlayerAttackProgressBar->SetPercent(1.0 - HDPlayerCharacter->GetAttackCooldownPercent());
						}
						else
						{
							PlayerAttackProgressBar->SetVisibility(ESlateVisibility::Hidden);
						}
					}
					
					if (UProgressBar* PlayerMineProgressBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("Mine_CoolDown"))))
					{
						float Cooldown = HDPlayerCharacter->GetMineCooldownPercent();
						if (Cooldown > 0.0f)
						{
							PlayerMineProgressBar->SetVisibility(ESlateVisibility::Visible);
							PlayerMineProgressBar->SetPercent(1.0 - HDPlayerCharacter->GetMineCooldownPercent());
						}
						else
						{
							PlayerMineProgressBar->SetVisibility(ESlateVisibility::Hidden);
						}
					}
				}

				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("남은시간 : %.f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GetGameInstance()))
					{
						ScoreText->SetText(FText::FromString(FString::Printf(TEXT("점수: %d"), HDGameInstance->TotalScore)));
					}
				}

				if (UTextBlock* StageIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("StageText"))))
				{
					StageIndexText->SetText(FText::FromString(FString::Printf(TEXT("제 %d 장"), CurrentLevelIndex + 1)));
				}
				
				if (UTextBlock* GameRuleText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("GameRuleText"))))
				{
					GameRuleText->SetText(FText::FromString(FString::Printf(TEXT("괴물들의 공격을 피하고 사살하여 점수를 얻고 끝까지 생존하라"))));
					if (CurrentLevelIndex >= 1)
					{
						GameRuleText->SetVisibility(ESlateVisibility::Hidden);
					}
				}
			}
		}
	}
}

void AHDGameState::StartLevel()
{

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
			CurrentLevelIndex = HDGameInstance->CurrentLevelIndex;
		}
	}
	
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 MonsterToSpawn = (CurrentLevelIndex+1)*3;

	if (FoundVolumes.Num() > 0)
	{
		for (int32 i = 0; i < MonsterToSpawn; i++)
		{
			int32 TargetIdx = FMath::RandRange(0, FoundVolumes.Num() - 1);
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[TargetIdx]);

			if (SpawnVolume)
			{
				AHDMonCharacter* SpawnedMonster = SpawnVolume->SpawnRandomMonster();
				UE_LOG(LogTemp, Warning, TEXT("Spawned at Volume Index: %d"), TargetIdx);
			}
		}
	}
	
	const int32 HealItemToSpawn = 5;
	
	if (FoundVolumes.Num() > 0)
	{
		for (int32 i = 0; i < HealItemToSpawn; i++)
		{
			int32 ItemTargetIdx = FMath::RandRange(0, FoundVolumes.Num() - 1);
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[ItemTargetIdx]);

			if (SpawnVolume)
			{
				SpawnVolume->SpawnHealingItem();
				UE_LOG(LogTemp, Warning, TEXT("Spawned at Volume Index: %d"), ItemTargetIdx);
			}
		}
	}
	
	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AHDGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);

}

void AHDGameState::OnLevelTimeUp()
{
	EndLevel();
}

void AHDGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);

		if (HDGameInstance)
		{
			CurrentLevelIndex++;
			HDGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{
		GameClear();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AHDGameState::OnGameOver()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);

		if (HDGameInstance)
		{
			CurrentLevelIndex = 0;
			HDGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			HDPlayerController->SetPause(true);
			HDPlayerController->ShowGameOverUI();
		}
	}
}

void AHDGameState::GameClear()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);
		
		if (HDGameInstance)
		{
			CurrentLevelIndex = 0;
			HDGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AHDPlayerController* HDPlayerController = Cast<AHDPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = HDPlayerController->GetHUDWidget())
			{
				if (UGameInstance* GameInstance = GetGameInstance())
				{
					UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GameInstance);
					
					if (UTextBlock* TotalScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TotalScoreText"))))
					{
						float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
						TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("최종 점수 : %d"), HDGameInstance->TotalScore)));
					}
				}
			}
			HDPlayerController->SetPause(true);
			HDPlayerController->ShowGameClearUI();
		}
	}
}