// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "BatteryWidget_cpp.generated.h"


/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBatteryWidget_cpp : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
//	UUISlot_cpp(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
		void SetBatteryPercent(int32 value);

	UFUNCTION()
		void SetBatteryVisibility(bool value);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Battery;
		
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BatteryImage;
};