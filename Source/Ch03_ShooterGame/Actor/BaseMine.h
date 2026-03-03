// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MineInterface.h"
#include "BaseMine.generated.h"


class USphereComponent;

UCLASS()
class CH03_SHOOTERGAME_API ABaseMine : public AActor, public IMineInterface 
{
	GENERATED_BODY()
	
public:	
	ABaseMine();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* PickupParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* PickupSound;


	
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

};
