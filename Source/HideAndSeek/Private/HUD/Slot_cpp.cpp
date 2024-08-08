// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/Slot_cpp.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

// 초기화하는 함수.
void USlot_cpp::Init()
{
	// 상태를 업데이트 함.
	Refresh();
}

// 슬롯 타입을 설정할 함수.
void USlot_cpp::SetType(ESlotType type)
{
	Type = type;
}

// 슬롯의 텍스쳐를 설정할 함수.
void USlot_cpp::SetTexture(UTexture2D* texture)
{
	if (texture == nullptr) return;
	Icon->SetBrushFromTexture(texture);
}

// 업데이트하는 함수.
void USlot_cpp::Refresh()
{
	switch (Type)
	{
		// 타입이 아이템이면
		case ESlotType::SLOT_Item:
		{
			// 플레이어의 인벤토리로부터 데이터를 가져옴.
			FHorrorGameItemData& data = Player->Inventory[SlotNum];

			// 아이콘이 있다면 아이콘을 설정해줌.
			if (data.ItemIcon != nullptr)
			{
				SetTexture(data.ItemIcon);
			}

			// 개수도 설정해줌.
			Count = data.ItemCount;

			// 개수가 0 이하일 경우, 아이콘도, 선택 창도 안 보이게 설정함.
			if (Count < 1)
			{
				Icon->SetVisibility(ESlateVisibility::Collapsed);
				Have->SetVisibility(ESlateVisibility::Collapsed);
				Select_Border_Top->SetVisibility(ESlateVisibility::Collapsed);
				Select_Border_Left->SetVisibility(ESlateVisibility::Collapsed);
				Select_Border_Bottom->SetVisibility(ESlateVisibility::Collapsed);
				Select_Border_Right->SetVisibility(ESlateVisibility::Collapsed);
				/*Icon->SetVisibility(ESlateVisibility::Hidden);
				Have->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Top->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Left->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Bottom->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Right->SetVisibility(ESlateVisibility::Hidden);*/
			}
			// 개수가 1개 이상일 경우
			else
			{
				// 아이콘과, 개수가 보이게 설정함.
				Icon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Have->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				Have->SetText(FText::FromString(FString::FromInt(Count)));
				
				// 현재 플레이어가 선택한 아이템일 경우 선택 창이 보이게 설정함.
				if (SlotNum == Player->CurrentItemNum)
				{
					Select_Border_Top->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Left->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Bottom->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Right->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Top->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Select_Border_Left->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Select_Border_Bottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					Select_Border_Right->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				// 현재 플레이어가 선택하지 않은 아이템일 경우 선택 창이 안 보이게 설정함.
				else
				{
					Select_Border_Top->SetVisibility(ESlateVisibility::Hidden);
					Select_Border_Left->SetVisibility(ESlateVisibility::Hidden);
					Select_Border_Bottom->SetVisibility(ESlateVisibility::Hidden);
					Select_Border_Right->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			break;
		}
	}
}
