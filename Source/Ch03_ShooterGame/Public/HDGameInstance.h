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
	int32 CurrentStageIndex;
};
