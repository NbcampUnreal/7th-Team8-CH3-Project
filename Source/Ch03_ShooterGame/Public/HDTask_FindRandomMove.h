
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "HDTask_FindRandomMove.generated.h"


UCLASS()
class CH03_SHOOTERGAME_API UHDTask_FindRandomMove : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UHDTask_FindRandomMove();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector PatrolLocationKey;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 1000.0f;
};
