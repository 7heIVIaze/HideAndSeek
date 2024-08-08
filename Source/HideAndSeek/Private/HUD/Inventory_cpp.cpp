// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/Inventory_cpp.h"
#include "HUD/Slot_cpp.h"
#include "Blueprint/WidgetTree.h"

// 인벤토리 초기화 함수.
void UInventory_cpp::Init()
{
	// 인벤토리 9칸을 확인해 아무것도 없는 상태면 빈 아이콘으로 초기화함.
	for (int i = 0; i < 9; i++)
	{
		if (Player->Inventory[i].Type == EItemType::ITEM_None)
		{
			Player->Inventory[i].ItemIcon = DefaultTexture;
		}
	}

	// 슬롯을 초기화함.
	Slots.Init(nullptr, 9);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot_cpp* slot = nullptr;

	// 슬롯을 하나씩 Slot Widget를 캐스팅해서 초기화해줌.
	for (auto widget : widgets)
	{
		slot = Cast<USlot_cpp>(widget);
			
		if (!slot) continue;
		slot->Player = this->Player;
		slot->SetType(ESlotType::SLOT_Item);
		slot->Init();
		Slots[slot->SlotNum] = slot;
	}
}