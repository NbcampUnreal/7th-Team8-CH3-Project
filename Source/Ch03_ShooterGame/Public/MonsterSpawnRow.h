#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FMonsterSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MonsterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ACharacter> MonsterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpawnChance;

};
