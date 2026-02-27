#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HDGameInstance.generated.h"

UCLASS()
class CH03_SHOOTERGAME_API UHDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UHDGameInstance();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};
