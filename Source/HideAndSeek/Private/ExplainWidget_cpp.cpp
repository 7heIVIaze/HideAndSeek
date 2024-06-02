// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "ExplainWidget_cpp.h"
#include "Components/TextBlock.h"

void UExplainWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();

	ExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Explain_Text")));
}

void UExplainWidget_cpp::SetExplainText(FText strText)
{
	ExplainText->SetText(strText);
}