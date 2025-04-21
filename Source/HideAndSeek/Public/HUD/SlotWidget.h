// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "SlotWidget.generated.h"

//class UImage;
//class UTextBlock;
//class UBorder;
//class FRelpy;
//
UENUM(BlueprintType)
enum ESlotType
{
	SLOT_None	UMETA(DisplayName = "None"),
	SLOT_ConsumableItem	UMETA(DisplayName = "ConsumableItem"),
	SLOT_LightingItem	UMETA(DisplayName = "LightingItem"),
	SLOT_PassiveItem	UMETA(DisplayName = "PassiveItem"),
};

UCLASS()
class HIDEANDSEEK_API USlotWidget : public UCustomUI
{
	GENERATED_BODY()
	
public:
	void Init() override;

	virtual void Refresh() override;

	UFUNCTION(BlueprintCallable)
	void SetType(ESlotType inType);

	UFUNCTION(BlueprintCallable)
	void SetSlotTexture(UTexture2D* Texture);

	UFUNCTION(BlueprintCallable)
	virtual void SelectedItemSlot(int inSlotNum);

	UFUNCTION(BlueprintCallable)
	virtual void SetItemDurability(float inDurability);

	// false: Collapsed, true: SelfHitTestInvisible
	UFUNCTION(BlueprintCallable)
	void SetActivate(bool inIsActive);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 5, UIMin = 0))
	int SlotNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Have;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBorder* SelectedBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* DurabilityBar;
};
