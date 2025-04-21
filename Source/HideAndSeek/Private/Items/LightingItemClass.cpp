// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/LightingItemClass.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
ALightingItemClass::ALightingItemClass()
{
 	
}

// 플레이어가 타이머 아이템을 습득하려 할 때 작동할 함수.
void ALightingItemClass::OnInteract(class AHorrorGameCharacter* Player) // Player Click Event
{
	bool Result = Player->TakeItems(this);

	if (Result)
	{
		Super::OnInteract(Player);
	}
}

bool ALightingItemClass::UseInteract(class AHorrorGameCharacter* Player)
{
	return true;
}

void ALightingItemClass::HoldInteract(class AHorrorGameCharacter* Player)
{
	bool Result = Player->ChangeItems(this);

	if (Result)
	{
		// Play Sound

		// Move to Player And Destroy
		Super::HoldInteract(Player);
	}
}

FHorrorGameItemData ALightingItemClass::GetItemData()
{
	Super::GetItemData();

	return ItemData;
}