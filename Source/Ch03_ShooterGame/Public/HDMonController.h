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
	virtual void OnUnPossess()override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;

	virtual void Tick(float DeltaTime) override;

private:
	void OnRepeatTimer();
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* BBAsset;

};
