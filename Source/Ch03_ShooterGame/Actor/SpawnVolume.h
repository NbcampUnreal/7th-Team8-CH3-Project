// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class CH03_SHOOTERGAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* MonsterDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AHDMonCharacter* SpawnRandomMonster();
    FMonsterSpawnRow* GetRandomMonster() const;
    AHDMonCharacter* SpawnMonster(TSubclassOf<AHDMonCharacter> MonsterClass);
    FVector GetRandomPointInVolume() const;

};
