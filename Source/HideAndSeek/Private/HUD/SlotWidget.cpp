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

// 구 버전
//void USlotWidget::Refresh()
//{
//	switch (Type)
//	{
//		// 타입이 아이템이면
//		case ESlotType::SLOT_Item:
//		{
//			// 플레이어의 인벤토리로부터 데이터를 가져옴.
//			FHorrorGameItemData& data = Player->Inventory[SlotNum];
//
//			// 아이콘이 있다면 아이콘을 설정해줌.
//			if (data.ItemIcon != nullptr)
//			{
//				SetTexture(data.ItemIcon);
//			}
//
//			// 개수도 설정해줌.
//			Count = data.ItemCount;
//
//			// 개수가 0 이하일 경우, 아이콘도, 선택 창도 안 보이게 설정함.
//			if (Count < 1)
//			{
//				Icon->SetVisibility(ESlateVisibility::Collapsed);
//				Have->SetVisibility(ESlateVisibility::Collapsed);
//				Select_Border_Top->SetVisibility(ESlateVisibility::Collapsed);
//				Select_Border_Left->SetVisibility(ESlateVisibility::Collapsed);
//				Select_Border_Bottom->SetVisibility(ESlateVisibility::Collapsed);
//				Select_Border_Right->SetVisibility(ESlateVisibility::Collapsed);
//				/*Icon->SetVisibility(ESlateVisibility::Hidden);
//				Have->SetVisibility(ESlateVisibility::Hidden);
//				Select_Border_Top->SetVisibility(ESlateVisibility::Hidden);
//				Select_Border_Left->SetVisibility(ESlateVisibility::Hidden);
//				Select_Border_Bottom->SetVisibility(ESlateVisibility::Hidden);
//				Select_Border_Right->SetVisibility(ESlateVisibility::Hidden);*/
//			}
//			// 개수가 1개 이상일 경우
//			else
//			{
//				// 아이콘과, 개수가 보이게 설정함.
//				Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				Have->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				Have->SetText(FText::FromString(FString::FromInt(Count)));
//				
//				// 현재 플레이어가 선택한 아이템일 경우 선택 창이 보이게 설정함.
//				if (SlotNum == Player->CurrentItemNum)
//				{
//					Select_Border_Top->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
//					Select_Border_Left->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
//					Select_Border_Bottom->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
//					Select_Border_Right->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
//					Select_Border_Top->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//					Select_Border_Left->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//					Select_Border_Bottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//					Select_Border_Right->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
//				}
//				// 현재 플레이어가 선택하지 않은 아이템일 경우 선택 창이 안 보이게 설정함.
//				else
//				{
//					Select_Border_Top->SetVisibility(ESlateVisibility::Hidden);
//					Select_Border_Left->SetVisibility(ESlateVisibility::Hidden);
//					Select_Border_Bottom->SetVisibility(ESlateVisibility::Hidden);
//					Select_Border_Right->SetVisibility(ESlateVisibility::Hidden);
//				}
//			}
//			break;
//		}
//	}
//}
