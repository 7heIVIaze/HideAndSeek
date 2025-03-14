// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Resource/CustomDataTables.h"
#include "Player/HorrorGameCharacter.h"
#include "HUD/GameUI.h"
#include "HUD/InventoryWidget.h"

// Sets default values
// 초기화하는 함수.
void FHorrorGameItemData::Clear()
{
	// 모든 데이터를 초기화함.
	ItemNumber = EItemNumber::ITEM_None;
	ItemName = NSLOCTEXT("ItemName", "None", "");
	ItemCount = 0;
	ItemIcon = nullptr;
	ItemType = EItemType::ITEM_NONE;
}

// 아이템을 사용할 때 호출할 함수. 현재는 deprecated 됨.
void FHorrorGameItemData::Use(AHorrorGameCharacter* Player)
{
	if (ItemType == EItemType::ITEM_Consumable)
	{
		ItemCount--;
		if (ItemCount <= 0)
		{
			this->Clear();
			ItemIcon = Player->GameUIWidget->InventoryWidget->DefaultTexture;
		}
	}
}