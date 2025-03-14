// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/InventoryWidget.h"
#include "HUD/SlotWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"

// 인벤토리 초기화 함수.
void UInventoryWidget::Init()
{
	ConsumableInventoryMaxSize = Player->GetInventoryComponent()->ConsumableInventoryMaxSize;
	LightingInventoryMaxSize = Player->GetInventoryComponent()->LightingInventoryMaxSize;
	PassiveInventoryMaxSize = Player->GetInventoryComponent()->PassiveInventoryMaxSize;
	
	ConsumableItemSlots.Init(nullptr, ConsumableInventoryMaxSize);
	LightingItemSlots.Init(nullptr, LightingInventoryMaxSize);
	PassiveItemSlots.Init(nullptr, PassiveInventoryMaxSize);

	TArray<UWidget*> Widgets;
	USlotWidget* SlotWidget = nullptr;

	Widgets = PassiveItemPanel->GetAllChildren();

	for (auto Widget : Widgets)
	{
		SlotWidget = Cast<USlotWidget>(Widget);

		if (!SlotWidget) continue;

		if (SlotWidget->SlotNum < PassiveInventoryMaxSize)
		{
			SlotWidget->Player = this->Player;
			SlotWidget->Init();

			PassiveItemSlots[SlotWidget->SlotNum] = SlotWidget;
		}
		// 
		else
		{
			SlotWidget->Player = this->Player;
			SlotWidget->SetActivate(false);
		}
	}

	auto PassiveItemPanelSlot = Cast<UCanvasPanelSlot>(PassiveItemPanel->Slot);
	if (PassiveItemPanelSlot)
	{
		PassiveItemPanelSlot->SetSize(FVector2D(80.0f, PassiveInventoryMaxSize * 80.0f + (PassiveInventoryMaxSize - 1) * 10.0f));
		PassiveItemPanelSlot->SetAlignment(FVector2D(-1.0f, 2.1f));
	}

	// Consumable(Active) item slot.
	Widgets = ConsumableItemPanel->GetAllChildren();

	for (auto Widget : Widgets)
	{
		SlotWidget = Cast<USlotWidget>(Widget);

		if (!SlotWidget) continue;

		if (SlotWidget->SlotNum < ConsumableInventoryMaxSize)
		{
			SlotWidget->Player = this->Player;
			SlotWidget->Init();

			ConsumableItemSlots[SlotWidget->SlotNum] = SlotWidget;
		}
		else
		{
			SlotWidget->Player = this->Player;
			SlotWidget->SetActivate(false);
		}
	}
	auto ConsumableItemPanelSlot = Cast<UCanvasPanelSlot>(ConsumableItemPanel->Slot);
	if (ConsumableItemPanelSlot)
	{
		ConsumableItemPanelSlot->SetSize(FVector2D(ConsumableInventoryMaxSize * 80.f + (ConsumableInventoryMaxSize - 1) * 10.0f, 80.0f));
	}

	// Lighting item slot.
	Widgets = LightingItemPanel->GetAllChildren();

	for (auto Widget : Widgets)
	{
		SlotWidget = Cast<USlotWidget>(Widget);

		if (!SlotWidget) continue;

		if (SlotWidget->SlotNum < LightingInventoryMaxSize)
		{
			SlotWidget->Player = this->Player;
			SlotWidget->Init();

			LightingItemSlots[SlotWidget->SlotNum] = SlotWidget;
		}
		else
		{
			SlotWidget->Player = this->Player;
			SlotWidget->SetActivate(false);
		}
	}
	auto LightingItemPanelSlot = Cast<UCanvasPanelSlot>(LightingItemPanel->Slot);
	if (LightingItemPanelSlot)
	{
		LightingItemPanelSlot->SetSize(FVector2D(80.0f, LightingInventoryMaxSize * 80.f + (LightingInventoryMaxSize - 1) * 10.0f));
		LightingItemPanelSlot->SetAlignment(FVector2D(2.0f, 1.42f));
	}
}

bool UInventoryWidget::InitializeWidget(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		// Bind the delegate.
		InventoryComponent->OnItemAcquired.AddDynamic(this, &UInventoryWidget::RefreshSelectedItemSlot);
		InventoryComponent->OnItemSwitch.AddDynamic(this, &UInventoryWidget::RefreshSelectedItemSlot);
		InventoryComponent->OnItemConsumed.AddDynamic(this, &UInventoryWidget::RefreshSelectedItemSlot);
		InventoryComponent->OnDurabilityChanged.AddDynamic(this, &UInventoryWidget::RefreshItemDurability);

		return true;
	}

	return false;
}

void UInventoryWidget::Refresh()
{
	for (int i = 0; i < PassiveInventoryMaxSize; ++i)
	{
		PassiveItemSlots[i]->Init();
	}

	for (int i = 0; i < ConsumableInventoryMaxSize; ++i)
	{
		ConsumableItemSlots[i]->Init();
	}

	for (int i = 0; i < LightingInventoryMaxSize; ++i)
	{
		LightingItemSlots[i]->Init();
	}
}

void UInventoryWidget::RefreshSelectedItemSlot(const FHorrorGameItemData& ItemData, int32 CurrentItemIndex)
{
	TArray<UWidget*> Widgets;
	USlotWidget* SlotWidget = nullptr;

	// Refresh to reflect the current inventory status.
	Refresh();

	// Check the item type to refresh the inventory status of the currently selected item.
	switch (ItemData.ItemType)
	{
		// If the item is Active(Consumable) Item.
		case EItemType::ITEM_Consumable:
		{
			for (int i = 0; i < ConsumableInventoryMaxSize; ++i)
			{
				ConsumableItemSlots[i]->SelectedItemSlot(CurrentItemIndex);
			}
			break;
		}
		// If the item is Lighting Item.
		case EItemType::ITEM_Lighting:
		{
			for (int i = 0; i < LightingInventoryMaxSize; ++i)
			{
				int SlotNumber = (CurrentItemIndex + i) % LightingInventoryMaxSize;
				LightingItemSlots[i]->SlotNum = SlotNumber;
				LightingItemSlots[i]->Refresh();
				LightingItemSlots[i]->SelectedItemSlot(CurrentItemIndex);
			}
			break;
		}
	}
}

void UInventoryWidget::RefreshItemDurability(const FHorrorGameItemData& ItemData, int32 CurrentItemIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("RefreshItemDurability called. Event Delegate success!"));
	float Durability = 0.0f;

	switch (ItemData.ItemNumber)
	{
		case EItemNumber::ITEM_FlashLight:
		{
			Durability = FMath::Clamp(ItemData.Durability / 100.0f, 0.0f, 1.0f);
			break;
		}
		case EItemNumber::ITEM_Sword:
		{
			Durability = FMath::Clamp(ItemData.Durability / 3.0f, 0.0f, 1.0f);
			break;
		}
		case EItemNumber::ITEM_Bell:
		{
			Durability = FMath::Clamp(ItemData.Durability / 100.0f, 0.0f, 1.0f);
			break;
		}
		case EItemNumber::ITEM_Extinguisher:
		{
			Durability = FMath::Clamp(ItemData.Durability / 6.0f, 0.0f, 1.0f);
			break;
		}
		case EItemNumber::ITEM_Cutter:
		{
			Durability = FMath::Clamp(ItemData.Durability / 5.0f, 0.0f, 1.0f);
			break;
		}
		default:
		{
			Durability = 0.0f;
			break;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("RefreshItemDurability/ Durability: %f"), Durability);

	switch (ItemData.ItemType)
	{
		// Consumable item slot.
		case EItemType::ITEM_Consumable:
		{
			ConsumableItemSlots[CurrentItemIndex]->SetItemDurability(Durability);
			break;
		}

		// Lighting item slot.
		case EItemType::ITEM_Lighting:
		{
			// The slot index of the currently selected lighting item is always 0.
			LightingItemSlots[0]->SetItemDurability(Durability);
			break;
		}

		// Passives item slot.
		case EItemType::ITEM_Passive:
		{
			PassiveItemSlots[CurrentItemIndex]->SetItemDurability(Durability);
		}

	}
}

void UInventoryWidget::SelectedItemSlot(int CurrentConsumableItemIndex, int CurrentLightingItemIndex, int CurrentPassiveItemIndex)
{
	TArray<UWidget*> Widgets;
	USlotWidget* SlotWidget = nullptr;

	// Passives item slot.
	if (CurrentPassiveItemIndex >= 0)
	{
		for (int i = 0; i < PassiveItemSlots.Num(); ++i)
		{
			PassiveItemSlots[i]->SelectedItemSlot(CurrentPassiveItemIndex);
		}
	}

	// Consumable(Active) item slot.
	if (CurrentConsumableItemIndex >= 0)
	{
		for (int i = 0; i < ConsumableInventoryMaxSize; ++i)
		{
			ConsumableItemSlots[i]->SelectedItemSlot(CurrentConsumableItemIndex);
		}
	}

	// Lighting item slot.

	// Swap current lighting item slots.
	if (this->Player->GetInventoryComponent()->LightingInventoryNumber == 2)
	{
		if (CurrentLightingItemIndex == 0)
		{
			LightingItemSlots[0]->SlotNum = 0;
			LightingItemSlots[0]->Refresh();

			LightingItemSlots[1]->SlotNum = 1;
			LightingItemSlots[1]->Refresh();
		}
		else if (CurrentLightingItemIndex == 1)
		{
			LightingItemSlots[0]->SlotNum = 1;
			LightingItemSlots[0]->Refresh();

			LightingItemSlots[1]->SlotNum = 0;
			LightingItemSlots[1]->Refresh();
		}
	}

	for (int i = 0; i < LightingItemSlots.Num(); ++i)
	{
		LightingItemSlots[i]->SelectedItemSlot(CurrentLightingItemIndex);
	}
}

void UInventoryWidget::SetItemDurability(int type, int index, int inDurability)
{
	// Consumable item slot.
	if (type == 0)
	{
		ConsumableItemSlots[index]->SetItemDurability(inDurability);
	}

	// Lighting item slot.
	else if (type == 1)
	{
		LightingItemSlots[index]->SetItemDurability(inDurability);
	}

	// Passives item slot.
	else if (type == 2)
	{
		PassiveItemSlots[index]->SetItemDurability(inDurability);
	}
}
