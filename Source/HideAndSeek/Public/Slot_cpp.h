// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "Slot_cpp.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class FRelpy;

UENUM(BlueprintType)
enum ESlotType
{
	SLOT_None	UMETA(DisplayName = "None"),
	SLOT_Item	UMETA(DisplayName = "Item"),
	SLOT_Quick	UMETA(DisplayName = "Quick"),
	SLOT_Q_Item	UMETA(DisplayName = "Q_Item"),
};

UCLASS()
class HIDEANDSEEK_API USlot_cpp : public UCustomUI
{
	GENERATED_BODY()
	
public:
	void Init() override;
	
	void SetType(ESlotType type);
	
	void SetTexture(UTexture2D* texture);

	void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 8, UIMin = 0))
		int SlotNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TEnumAsByte<ESlotType> Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Have;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* Select_Border_Top;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* Select_Border_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* Select_Border_Bottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UBorder* Select_Border_Right;
};
