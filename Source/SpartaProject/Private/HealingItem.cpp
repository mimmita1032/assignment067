#include "HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	HealingAmount = 20.0f;
	ItemType = "HealingItem";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealingAmount);
		}
		DestroyItem();
	}
	
}