// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "HDBTService_DetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API UHDBTService_DetectTarget : public UBTService
{
	GENERATED_BODY()
public:
	UHDBTService_DetectTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectRadius = 1000.0f;
};
