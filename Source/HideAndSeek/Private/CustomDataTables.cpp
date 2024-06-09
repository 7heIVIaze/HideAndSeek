// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "CustomDataTables.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "GameUI.h"
#include "Inventory_cpp.h"

// Sets default values
// 초기화하는 함수.
void FHorrorGameItemData::Clear()
{
	// 모든 데이터를 초기화함.
	ItemNumber = 0;
	ItemName = NSLOCTEXT("FHorrorGameItemData", "NULL", "");
	ItemCount = 0;
	ItemIcon = nullptr;
	Type = ITEM_None;
}

// 아이템을 사용할 때 호출할 함수. 현재는 deprecated 됨.
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

//void FHorrorGameItemData::Initialize()
//{
//
//}