// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChapterTitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UChapterTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		TObjectPtr<class UTextBlock> TitleText;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		TObjectPtr<class UTextBlock> ExplainText;

public:
	UFUNCTION(BlueprintCallable)
		void SetTitleText();

	UFUNCTION(BlueprintCallable)
		void SetExplainText();
};
