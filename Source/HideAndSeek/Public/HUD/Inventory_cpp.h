// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "Inventory_cpp.generated.h"

class USlot_cpp;
UCLASS()
class HIDEANDSEEK_API UInventory_cpp : public UCustomUI
{
	GENERATED_BODY()

	//bool AddItem(class FHorrorGameItemData& Item);
	
public:
//	UInventory_cpp(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	void Use(int index);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* DefaultTexture;

private:
	TArray<USlot_cpp*> Slots;
};
