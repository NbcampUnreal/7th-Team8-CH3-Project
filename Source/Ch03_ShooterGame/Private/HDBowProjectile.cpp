// Fill out your copyright notice in the Description page of Project Settings.


#include "HDBowProjectile.h"
// Sets default values
AHDBowProjectile::AHDBowProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHDBowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHDBowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

