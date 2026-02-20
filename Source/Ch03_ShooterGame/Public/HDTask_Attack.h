// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "HDTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class CH03_SHOOTERGAME_API UHDTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UHDTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//옵저버 어봇: 중단될 시 호출되는 함수. 
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 성공/실패/중단, 아무튼 무관하게 종료될 시
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
};
