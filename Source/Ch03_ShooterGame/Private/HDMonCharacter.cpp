// Fill out your copyright notice in the Description page of Project Settings.


#include "HDMonCharacter.h"
#include "HDMonController.h"

// Sets default values
AHDMonCharacter::AHDMonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AHDMonController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


}

// Called when the game starts or when spawned
void AHDMonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHDMonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHDMonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

