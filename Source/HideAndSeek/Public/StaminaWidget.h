// CopyrightNotice=0 2023 Sunggon Kim

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
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

private:
	UPROPERTY()
		class UProgressBar* Stamina;

public:
	UFUNCTION()
		void SetStamina(int32 iValue);
	
	UFUNCTION()
		float GetStamina();
};
