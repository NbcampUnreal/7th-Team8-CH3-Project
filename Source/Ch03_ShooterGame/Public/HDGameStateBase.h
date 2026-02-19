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
	
	int CurrentStageIndex;

	FTimerHandle StageTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	virtual void BeginPlay() override;
	void UpdateHUD();
};
