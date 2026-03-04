#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Actor/Character/HDMonCharacter.h"
#include "ACtor/HealingItem.h"
#include "MonsterSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FMonsterSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MonsterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AHDMonCharacter> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AHealingItem> HealingItemClass;
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemSpawnChance;
};
