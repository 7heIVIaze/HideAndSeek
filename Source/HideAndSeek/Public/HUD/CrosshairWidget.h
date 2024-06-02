// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

UCLASS()
class HIDEANDSEEK_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Crosshair;

private:
	bool DotVisible = false;

public:	// Function
	UFUNCTION()
		void SetCrossHairVisible(bool value);
};
