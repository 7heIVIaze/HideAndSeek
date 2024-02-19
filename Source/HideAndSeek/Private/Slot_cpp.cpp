// Fill out your copyright notice in the Description page of Project Settings.

#include "Slot_cpp.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void USlot_cpp::Init()
{
	Refresh();
}

void USlot_cpp::SetType(ESlotType type)
{
	Type = type;
}

void USlot_cpp::SetTexture(UTexture2D* texture)
{
	if (texture == nullptr) return;
	Icon->SetBrushFromTexture(texture);
}

void USlot_cpp::Refresh()
{
	switch (Type)
	{
		case ESlotType::SLOT_Item:
		{
			FHorrorGameItemData& data = Player->Inventory[SlotNum];

			if (data.ItemIcon != nullptr)
			{
				SetTexture(data.ItemIcon);
			}

			Count = data.ItemCount;

			if (Count < 1)
			{
				Icon->SetVisibility(ESlateVisibility::Hidden);
				Have->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Top->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Left->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Bottom->SetVisibility(ESlateVisibility::Hidden);
				Select_Border_Right->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				Icon->SetVisibility(ESlateVisibility::Visible);
				Have->SetVisibility(ESlateVisibility::Visible);
				Have->SetText(FText::FromString(FString::FromInt(Count)));
				if (SlotNum == Player->GetCurrentItemNumber())
				{
					Select_Border_Top->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Left->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Bottom->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Right->SetBrushColor(FLinearColor::FLinearColor(1.f, 1.f, 1.f, 1.f));
					Select_Border_Top->SetVisibility(ESlateVisibility::Visible);
					Select_Border_Left->SetVisibility(ESlateVisibility::Visible);
					Select_Border_Bottom->SetVisibility(ESlateVisibility::Visible);
					Select_Border_Right->SetVisibility(ESlateVisibility::Visible);
				}
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
