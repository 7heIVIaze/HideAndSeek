// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "BatteryWidget_cpp.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "HideAndSeek/HorrorGameCharacter.h"

void UBatteryWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();
	Battery = Cast<UProgressBar>(GetWidgetFromName("BatteryUI"));
	BatteryImage = Cast<UImage>(GetWidgetFromName("BatteryImg"));
}

void UBatteryWidget_cpp::SetBatteryPercent(int32 value)
{
	float percent = (float)value / 200.0f;
	Battery->SetPercent(percent);
}

void UBatteryWidget_cpp::SetBatteryVisibility(bool value)
{
	if (value)
	{
		Battery->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Battery->SetVisibility(ESlateVisibility::Collapsed);
	}
}