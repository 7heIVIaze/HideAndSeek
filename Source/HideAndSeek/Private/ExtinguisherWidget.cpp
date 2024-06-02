// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "ExtinguisherWidget.h"
#include "Components/ProgressBar.h"

void UExtinguisherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ExtBar = Cast<UProgressBar>(GetWidgetFromName("ExtinguisherLeft"));
}

void UExtinguisherWidget::SetExtinguisherPercent(int32 value)
{
	float percent = (float)value / 100.f;
	ExtBar->SetPercent(percent);
}