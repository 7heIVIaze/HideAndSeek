// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryWidget_cpp.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "HorrorGame/HorrorGameCharacter.h"

void UBatteryWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();
	Battery = Cast<UProgressBar>(GetWidgetFromName("BatteryUI"));
	BatteryImage = Cast<UImage>(GetWidgetFromName("BatteryImg"));
}

void UBatteryWidget_cpp::SetBatteryPercent(int32 value)
{
	float percent = (float)value / 100.0f;
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