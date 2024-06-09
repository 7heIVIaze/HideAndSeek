// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractDot_cpp.generated.h"

//class UInteractText_cpp;
//class UUseInteract_cpp;
UCLASS(Meta=(DisableNativeTick))
class HIDEANDSEEK_API UInteractDot_cpp : public UUserWidget
{
	GENERATED_BODY()
	
	//void InteractUIInit() override;

	//void Refresh();
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetInteractText(FText strText);

	UFUNCTION()
		void SetErrorText(FText strText);

	UFUNCTION()
		void SetInteractDot(bool Value);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* InteractDot;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* InteractText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ErrorText;
};
