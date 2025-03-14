// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "MessageManagerWidget.generated.h"

/**
 * 
 */

UCLASS()
class HIDEANDSEEK_API UMessageManagerWidget : public UCustomUI
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddMessage(FText Message, float Duration = 5.0f);

	UFUNCTION(BlueprintCallable)
	void CleanupMessages();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox>MessageListBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
	TSubclassOf<UUserWidget> MessageWidget;

private:
	UPROPERTY()
	TArray<class UMessageWidget*>MessageQueue;

	UPROPERTY()
	int MessageCount = 0;
};