// Fill out your copyright notice in the Description page of Project Settings.


#include "HDTask_Attack.h"
#include "AIController.h"
#include "GameFramework/Character.h"


UHDTask_Attack::UHDTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UHDTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		return EBTNodeResult::Aborted;
	}

	ACharacter* AiController = Cast<ACharacter>(Owner->GetPawn());
	if (AiController == nullptr || AttackMontage == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AiController->PlayAnimMontage(AttackMontage);
	return EBTNodeResult::InProgress;
}

void UHDTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	ACharacter* AICharacter = Cast<ACharacter>(Owner->GetPawn());
	if (AICharacter == nullptr || AttackMontage == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAnimInstance* AnimInstance = AICharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	bool bStopped = AnimInstance->Montage_GetIsStopped(AttackMontage);
	if (bStopped)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UHDTask_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UHDTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
