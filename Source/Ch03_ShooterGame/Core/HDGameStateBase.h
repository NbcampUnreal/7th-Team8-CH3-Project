#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HDGameStateBase.generated.h"

UCLASS()
class CH03_SHOOTERGAME_API AHDGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHDGameStateBase();
	


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	int CurrentStageIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxStages;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> StageMapNames;

	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle StageTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	virtual void BeginPlay() override;
	void UpdateHUD();
	void StartStage();
	void OnLevelTimeUp();
	void EndStage();
	void OnGameOver();
};
