// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* PanicBar;

public:
	UFUNCTION()
	void SetPatience(int32 value);

	UFUNCTION()
	float GetPatience();
};
