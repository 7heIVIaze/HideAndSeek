// CopyrightNotice=0 2023 Sunggon Kim

#include "ExplainWidget_cpp.h"
#include "Components/TextBlock.h"

void UExplainWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();

	ExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Explain_Text")));
}

void UExplainWidget_cpp::SetExplainText(FString strText)
{
	ExplainText->SetText(FText::FromString(strText));
}