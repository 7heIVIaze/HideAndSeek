// CopyrightNotice=0 2023 Sunggon Kim
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExplainWidget_cpp.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UExplainWidget_cpp : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetExplainText(FString strText);

private:
	UPROPERTY()
		class UTextBlock* ExplainText;
};
