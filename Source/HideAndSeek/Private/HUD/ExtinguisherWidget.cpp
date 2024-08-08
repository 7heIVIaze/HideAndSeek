// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ExtinguisherWidget.h"
#include "Components/ProgressBar.h"

void UExtinguisherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ExtBar = Cast<UProgressBar>(GetWidgetFromName("ExtinguisherLeftBar"));
}

// 소화기의 남은 양을 설정해줌.
void UExtinguisherWidget::SetExtinguisherPercent(int32 value)
{
	float percent = (float)value / 100.f;
	ExtinguisherLeftBar->SetPercent(percent);
}