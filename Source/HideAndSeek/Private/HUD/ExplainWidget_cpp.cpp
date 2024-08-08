// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ExplainWidget_cpp.h"
#include "Components/TextBlock.h"

void UExplainWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();

	//ExplainText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Explain_Text")));
}

// 설명 문구를 설정함.
void UExplainWidget_cpp::SetExplainText(FText strText)
{
	ExplainText->SetText(strText);
}