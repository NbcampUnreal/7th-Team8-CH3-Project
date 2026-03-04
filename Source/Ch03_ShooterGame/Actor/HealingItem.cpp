#include "Actor/HealingItem.h"
#include "Character/HDPlayerCharacter.h"


AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHDPlayerCharacter* HDPlayerCharacter = Cast<AHDPlayerCharacter>(Activator))
		{
			HDPlayerCharacter->AddHealth(HealAmount);
		}
		
		DestroyItem();
	}
}
