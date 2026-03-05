#pragma once

#include "CoreMinimal.h"
#include "Actor/BaseMine.h"
#include "HealingItem.generated.h"

UCLASS()
class CH03_SHOOTERGAME_API AHealingItem : public ABaseMine
{
	GENERATED_BODY()
	
public:
	AHealingItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealAmount;
	
	virtual void ActivateItem(AActor* Activator) override;
};
