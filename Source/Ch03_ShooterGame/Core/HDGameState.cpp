// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameState.h"
#include "HDGameInstance.h"
#include "Actor/Character/HDPlayerCharacter.h"
#include "Actor/Character/HDPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

AHDGameState::AHDGameState()
{
	Score = 0;
	LevelDuration = 60.0f; 
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

int32 AHDGameState::GetScore() const
{
	return Score;
}

void AHDGameState::AddScore(int32 Amount)
{
	Score += Amount;
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
			CurrentStageIndex = HDGameInstance->CurrentStageIndex;
		}
	}
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	//TArray<AActor*> FoundVolumes;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	//const int32 ItemToSpawn = 40;

	//for (int32 i = 0; i < ItemToSpawn; i++)
	//{
		//if (FoundVolume.Num() > 0)
		//{
		//	ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		//	if (SpawnVolume)
		//	{
		//		AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
		//		// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
		//		if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
		//		{
		//			SpawnedCoinCount++;
		//		}
		//	}
		//}
	//}

	// 30초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
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
	// 모든 레벨을 다 돌았다면 게임 오버 처리
	//if (CurrentLevelIndex >= MaxLevels)
	//{
	//	OnGameOver();
	//	return;
	//}

	//// 레벨 맵 이름이 있다면 해당 맵 불러오기
	//if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	//{
	//	UGamePlayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	//}
	//else
	//{
	//	// 맵 이름이 없으면 게임오버
	//	OnGameOver();
	//}
}

void AHDGameState::OnGameOver()
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