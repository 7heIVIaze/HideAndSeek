// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "ExtinguisherWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UExtinguisherWidget : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetExtinguisherPercent(int32 value);

public:
	UPROPERTY()
		class UProgressBar* ExtBar;
};
