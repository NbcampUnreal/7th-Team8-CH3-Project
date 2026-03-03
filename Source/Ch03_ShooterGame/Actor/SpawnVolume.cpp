// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

AHDMonCharacter* ASpawnVolume::SpawnRandomMonster()
{
    if (FMonsterSpawnRow* SelectedRow = GetRandomMonster())
    {
        if (UClass* ActualClass = SelectedRow->MonsterClass.Get())
        {
            return SpawnMonster(ActualClass);
        }
    }
    return nullptr;
}

FMonsterSpawnRow* ASpawnVolume::GetRandomMonster() const
{
    if (!MonsterDataTable) return nullptr;

    TArray<FMonsterSpawnRow*> AllRows;
    static const FString ContextString(TEXT("MonsterSpawnContext"));
    MonsterDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    float TotalChance = 0.0f;
    for (const FMonsterSpawnRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FMonsterSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

AHDMonCharacter* ASpawnVolume::SpawnMonster(TSubclassOf<AHDMonCharacter> MonsterClass)
{
    if (!MonsterClass) return nullptr;
    AHDMonCharacter* SpawnedMonster = GetWorld()->SpawnActor<AHDMonCharacter>(
        MonsterClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );

    return SpawnedMonster;
}

void ASpawnVolume::SpawnHealingItem(TSubclassOf<AActor> AHealingItem)
{
    if (!AHealingItem) return;

    GetWorld()->SpawnActor<AActor>(
        AHealingItem,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );
}
