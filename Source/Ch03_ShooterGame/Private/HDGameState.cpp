// Fill out your copyright notice in the Description page of Project Settings.


#include "HDGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"

AHDGameState::AHDGameState()
{
	Score = 0;
	LevelDuration = 60.0f; // 레벨당 초
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AHDGameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
	StartLevel();
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

void AHDGameState::StartLevel()
{
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
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AHDGameState::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 다음 레벨 인덱스로
	CurrentLevelIndex++;

	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

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
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
	// 여기서 UI를 띄운다거나, 재시작 기능을 넣을 수도 있음
}