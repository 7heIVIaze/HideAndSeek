// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
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
