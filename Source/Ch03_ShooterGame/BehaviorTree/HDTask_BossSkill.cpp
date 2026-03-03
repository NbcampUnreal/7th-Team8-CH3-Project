// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/HDTask_BossSkill.h"
#include "Actor/Character/HDMonCharacter.h"
#include "Actor/Character/HDMonController.h"

UHDTask_BossSkill::UHDTask_BossSkill()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UHDTask_BossSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		return EBTNodeResult::Aborted;
	}
	
	AHDMonCharacter* Boss = Cast<AHDMonCharacter>(Owner->GetPawn());
	if (Boss->SkillReadyIsActive())
	{
		Boss->WaitSkill();
    
		return EBTNodeResult::InProgress;
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UHDTask_BossSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Owner = OwnerComp.GetAIOwner();
	if (Owner == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	auto Boss = Cast<AHDMonCharacter>(Owner->GetPawn());
	if (Boss == nullptr || AttackMontage == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
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

EBTNodeResult::Type UHDTask_BossSkill::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UHDTask_BossSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}
