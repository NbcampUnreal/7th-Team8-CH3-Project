// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HDGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API AHDGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHDGameStateBase();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentStageIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float StageDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxStages;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> StageMapNames;

	FTimerHandle StageTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	void UpdateHUD();
	void StartLevel();
	void OnStageTimeUp();
	void EndStage();
	void OnGameOver();
};
