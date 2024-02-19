// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_cpp.h"
#include "Slot_cpp.h"
#include "Blueprint/WidgetTree.h"

//UInventory_cpp::UInventory_cpp(const FObjectInitializer& ObjectInitializer)
//{
//	
//}

void UInventory_cpp::Init()
{
	for (int i = 0; i < 9; i++)
	{
		if (Player->Inventory[i].Type == EItemType::ITEM_None)
		{
			Player->Inventory[i].ItemIcon = DefaultTexture;
		}
	}

	Slots.Init(nullptr, 9);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot_cpp* slot = nullptr;
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