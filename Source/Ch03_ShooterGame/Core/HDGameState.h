// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HDGameState.generated.h"


UCLASS()
class CH03_SHOOTERGAME_API AHDGameState : public AGameState
{
	GENERATED_BODY()

public:	
	
	AHDGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	int32 MonsterToSpawn;
	int32 BossStageMonsterToSpawn;
	int32 KillCount;
	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle LevelTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	void AddKillCount(int32 Amount);

	virtual void BeginPlay() override;
	void UpdateHUD();
	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	void GameClear();
};
