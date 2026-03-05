// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMine.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API AMineItem : public ABaseMine
{
	GENERATED_BODY()
	
public:
	AMineItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* ExplosionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay; //폭발시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")

	float ExplosionRadius; //폭발범위

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDamage; //데미지


	bool bHasExploded;
	// 지뢰 발동 여부
	FTimerHandle ExplosionTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;


	void Explode();
};
