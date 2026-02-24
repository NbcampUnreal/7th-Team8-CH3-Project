// Fill out your copyright notice in the Description page of Project Settings.


#include "HDBTService_DetectTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UHDBTService_DetectTarget::UHDBTService_DetectTarget()
{
	NodeName = TEXT("Detect Target");
	Interval = 0.5f;
	RandomDeviation = 0.1f;

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UHDBTService_DetectTarget, TargetActorKey), AActor::StaticClass());
}

void UHDBTService_DetectTarget::TickNode(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AiController = OwnerComp.GetAIOwner();
	if (AiController == nullptr || AiController->GetPawn() == nullptr)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TargetActorKey.SelectedKeyName);
		return;
	}

	const float Distance = FVector::Dist(
		AiController->GetPawn()->GetActorLocation(),
		PlayerPawn->GetActorLocation());

	if (Distance <= DetectRadius)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, PlayerPawn);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(TargetActorKey.SelectedKeyName);
	}
}
