// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/SlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Resource/CustomDataTables.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"

// 초기화하는 함수.
void USlotWidget::Init()
{
	// 상태를 업데이트 함.
	Refresh();
}

// 슬롯 타입을 설정할 함수.
void USlotWidget::SetType(ESlotType inType)
{
	Type = inType;
}

// 슬롯의 텍스쳐를 설정할 함수.
void USlotWidget::SetSlotTexture(UTexture2D* Texture)
{
	if (Texture == nullptr)
	{
		return;
	}

	Icon->SetBrushFromTexture(Texture);
}

// 업데이트하는 함수.
void USlotWidget::Refresh()
{
	switch (Type)
	{
		case SLOT_ConsumableItem:
		{
			if (SlotNum < Player->GetInventoryComponent()->ConsumableInventoryMaxSize)
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Consumable_Inventory[SlotNum];

				if (ItemData.ItemIcon != nullptr)
				{
					SetSlotTexture(ItemData.ItemIcon);
				}

				Count = ItemData.ItemCount;

				// There is no item.
				if (Count < 1)
				{
					Icon->SetVisibility(ESlateVisibility::Collapsed);
					Have->SetVisibility(ESlateVisibility::Collapsed);
				}
				// The item in slot is 1 or more
				else
				{
					Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetText(FText::FromString(FString::FromInt(Count)));
				}
			}

			break;
		}
		case SLOT_LightingItem:
		{
			if (SlotNum < Player->GetInventoryComponent()->LightingInventoryMaxSize)
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Lighting_Inventory[SlotNum];

				if (ItemData.ItemIcon != nullptr)
				{
					SetSlotTexture(ItemData.ItemIcon);
				}

				Count = ItemData.ItemCount;

				// There is no item.
				if (Count < 1)
				{
					Icon->SetVisibility(ESlateVisibility::Collapsed);
					Have->SetVisibility(ESlateVisibility::Collapsed);
				}
				// The item in slot is 1 or more
				else
				{
					Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetText(FText::FromString(FString::FromInt(Count)));
				}
			}
			break;
		}
		case SLOT_PassiveItem:
		{
			if (SlotNum < Player->GetInventoryComponent()->PassiveInventoryMaxSize)
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Passive_Inventory[SlotNum];

				if (ItemData.ItemIcon != nullptr)
				{
					SetSlotTexture(ItemData.ItemIcon);
				}

				Count = ItemData.ItemCount;

				// There is no item.
				if (Count < 1)
				{
					Icon->SetVisibility(ESlateVisibility::Collapsed);
					Have->SetVisibility(ESlateVisibility::Collapsed);
				}
				// The item in slot is 1 or more
				else
				{
					Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Have->SetText(FText::FromString(FString::FromInt(Count)));
				}
			}
			break;
		}
	}
}

void USlotWidget::SelectedItemSlot(int inSlotNum)
{
	if (inSlotNum < 0)
	{
		return;
	}

	// Execute this function if the slot number is same. This means that the the player select a item in this inventory slot
	if (SlotNum == inSlotNum)
	{
		SelectedBorder->SetBrushColor(FLinearColor(1.0f, 0.4f, 0.4f, 0.5f));

		// Check the slot type to get the item data from the inventory where the item is located.
		switch (Type)
		{
			case ESlotType::SLOT_ConsumableItem:
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Consumable_Inventory[SlotNum];

				// If the item has durability
				if (ItemData.Durability != -1)
				{
					DurabilityBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					float Durability = 0.0f;

					switch (ItemData.ItemNumber)
					{
						case EItemNumber::ITEM_Sword:
						{
							Durability = FMath::Clamp(ItemData.Durability / 3.0f, 0.0f, 1.0f);
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
						// Items without durability do not need to have their durability bar values ​​set.
						default:
						{
							return;
						}
					}

					SetItemDurability(Durability);
				}
				// If the item does not have durability
				else
				{
					DurabilityBar->SetVisibility(ESlateVisibility::Collapsed);
				}
				break;
			}
			case ESlotType::SLOT_LightingItem:
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Lighting_Inventory[SlotNum];

				// If the item has durability
				if (ItemData.Durability != -1)
				{
					DurabilityBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					float Durability = 0.0f;

					switch (ItemData.ItemNumber)
					{
						case EItemNumber::ITEM_FlashLight:
						{
							Durability = FMath::Clamp(ItemData.Durability / 100.0f, 0.0f, 1.0f);
							break;
						}
						// Items without durability do not need to have their durability bar values ​​set.
						default:
						{
							return;
						}
					}

					SetItemDurability(Durability);
				}
				// If the item does not have durability
				else
				{
					DurabilityBar->SetVisibility(ESlateVisibility::Collapsed);
				}
				break;
			}
			case ESlotType::SLOT_PassiveItem:
			{
				FHorrorGameItemData& ItemData = Player->GetInventoryComponent()->Passive_Inventory[SlotNum];

				// If the item has durability
				if (ItemData.Durability != -1)
				{
					DurabilityBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					float Durability = 0.0f;

					switch (ItemData.ItemNumber)
					{
						case EItemNumber::ITEM_Bell:
						{
							Durability = FMath::Clamp(ItemData.Durability / 100.0f, 0.0f, 1.0f);
							break;
						}
						// Items without durability do not need to have their durability bar values ​​set.
						default:
						{
							return;
						}
					}

					SetItemDurability(Durability);
				}
				// If the item does not have durability
				else
				{
					DurabilityBar->SetVisibility(ESlateVisibility::Collapsed);
				}
				break;
			}
			default:
			{
				DurabilityBar->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
	// Else, then make the border's opacity 0
	else
	{
		SelectedBorder->SetBrushColor(FLinearColor(1.0f, 0.4f, 0.4f, 0.0f));

		DurabilityBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotWidget::SetItemDurability(float inDurability)
{
	DurabilityBar->SetPercent(inDurability);
}

void USlotWidget::SetActivate(bool inIsActive)
{
	// Set Visible
	if (inIsActive)
	{
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	// Collapsed
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}