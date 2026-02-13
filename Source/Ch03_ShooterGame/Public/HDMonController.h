// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "HDMonController.generated.h"

class UBehaviorTree;
class UBlackboardData;
/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API AHDMonController : public AAIController
{
	GENERATED_BODY()
	

public:
	AHDMonController();
	
	virtual void OnPossess(APawn* InPawn) override;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

};
