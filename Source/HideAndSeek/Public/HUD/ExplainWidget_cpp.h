// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExplainWidget_cpp.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UExplainWidget_cpp : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetExplainText(FText strText);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* ExplainText;
};
