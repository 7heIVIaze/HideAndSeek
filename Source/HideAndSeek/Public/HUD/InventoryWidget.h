// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "InventoryWidget.generated.h"

class USlot_cpp;
UCLASS()
class HIDEANDSEEK_API UInventoryWidget : public UCustomUI
{
	GENERATED_BODY()

	
public:
	void Init() override;
	void Use(int index) {};

	virtual void Refresh() override;

	//void SetQuickItem();
	UFUNCTION(BlueprintCallable)
	virtual void SelectedItemSlot(int CurrentUseableItemIndex, int CurrentLightingItemIndex, int CurrentEquipmentItemIndex);

	UFUNCTION(BlueprintCallable)
	void RefreshSelectedItemSlot(const FHorrorGameItemData& ItemData, int32 CurrentItemIndex);

	// type : 0 = Useable, 1 = Lighting, 2 = Equipment
	UFUNCTION(BlueprintCallable)
	virtual void RefreshItemDurability(const FHorrorGameItemData& ItemData, int32 CurrentItemIndex);

	// type : 0 = Useable, 1 = Lighting, 2 = Equipment
	UFUNCTION(BlueprintCallable)
	virtual void SetItemDurability(int type, int Index, int inDurability);

	// Initialize the inventory component for delegate binding.
	UFUNCTION(BlueprintCallable)
	bool InitializeWidget(class UInventoryComponent* InventoryComponent);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefaultTexture;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* PassiveItemPanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* ConsumableItemPanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* LightingItemPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int ConsumableInventoryMaxSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int LightingInventoryMaxSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int PassiveInventoryMaxSize;
private:
	TArray<class USlotWidget*> Slots;

	// Inventory Item Slots.
	UPROPERTY()
	TArray<class USlotWidget*> LightingItemSlots;
	UPROPERTY()
	TArray<class USlotWidget*> ConsumableItemSlots;
	UPROPERTY()
	TArray<class USlotWidget*> PassiveItemSlots;
};
