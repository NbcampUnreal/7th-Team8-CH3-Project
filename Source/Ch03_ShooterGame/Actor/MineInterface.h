// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MineInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CH03_SHOOTERGAME_API IMineInterface
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) = 0; //몬스터 진입했을때 발동되도록
	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) = 0; //EndOverlap 물체가 나갔을때.
	virtual void ActivateItem(AActor* Activator) = 0; //아이템이 실질적으로 사용이 될때 호출
	virtual FName GetItemType() const = 0; //string 보단 FName 가벼움

};
