// CopyrightNotice=0 2023 Sunggon Kim

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "PatienceWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UPatienceWidget : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UProgressBar* Mental;

public:
	UFUNCTION()
		void SetPatience(int32 value);

	UFUNCTION()
		float GetPatience();
};
