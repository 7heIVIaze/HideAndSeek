// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "CustomDataTables.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "GameUI.h"
#include "Inventory_cpp.h"

// Sets default values

void FHorrorGameItemData::Clear()
{
	ItemNumber = 0;
	ItemName = NSLOCTEXT("FHorrorGameItemData", "NULL", "");
	ItemCount = 0;
	ItemIcon = nullptr;
	Type = ITEM_None;
}

void FHorrorGameItemData::Use(AHorrorGameCharacter* Player)
{
	if (Type == EItemType::ITEM_Useable)
	{
		ItemCount--;
		if (ItemCount <= 0)
		{
			this->Clear();
			ItemIcon = Player->GameUIWidget->Inventory->DefaultTexture;
		}
	}
}