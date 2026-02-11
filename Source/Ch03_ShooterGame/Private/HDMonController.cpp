// Fill out your copyright notice in the Description page of Project Settings.


#include "HDMonController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AHDMonController::HomePosKey("HomePos");
const FName AHDMonController::PatrolPosKey("PatrolPos");



AHDMonController::AHDMonController()
{
	RepeatInterval = 3.0f;

	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT(""));
	if (BBObject.Succeeded()) {
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT(""));
	if (BTObject.Succeeded()) {
		BTAsset = BTObject.Object;
	}
}


void AHDMonController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	GetWorld()->GetTimerManager().SetTimer(
		RepeatTimerHandle,
		this,
		&AHDMonController::OnRepeatTimer,
		RepeatInterval,
		true
	);
	UBlackboardComponent* BlackboardComp = nullptr;

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		// 2. 비헤이비어 트리 실행
		BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			// 실행 실패 시 로그 출력
			UE_LOG(LogTemp, Error, TEXT("BT를 실행할 수 없습니다!"));
		}
	}
}

void AHDMonController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AHDMonController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (!IsValid(CurrentPawn))
	{
		return;
	}





	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem)return;

	FNavLocation NextLocation;

	if (NavSystem->GetRandomReachablePointInRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		
	}
}
