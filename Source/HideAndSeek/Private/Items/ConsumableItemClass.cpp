// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/ConsumableItemClass.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
AConsumableItemClass::AConsumableItemClass()
{
 	
}

void AConsumableItemClass::OnInteract(class AHorrorGameCharacter* Player)
{
	bool Result = Player->TakeItems(this);

	if (Result)
	{
		// 소리 재생
		Super::OnInteract(Player);
		Destroy();
	}
}

bool AConsumableItemClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return false;
}

void AConsumableItemClass::HoldInteract(class AHorrorGameCharacter* Player)
{
	bool Result = Player->ChangeItems(this);

	if (Result)
	{
		// 획득 소리 재생
		Super::HoldInteract(Player);
		Destroy();
	}
}

FHorrorGameItemData AConsumableItemClass::GetItemData()
{
	Super::GetItemData();

	return ItemData;
};