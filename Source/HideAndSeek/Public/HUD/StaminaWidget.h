// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "StaminaWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UStaminaWidget : public UCustomUI
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* StaminaBar;

public:
	UFUNCTION()
		void SetStamina(int32 iValue);
	
	UFUNCTION()
		float GetStamina();
};
