// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/TimerWidget.h"
#include "Components/TextBlock.h"

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Hour->SetVisibility(ESlateVisibility::Collapsed);

}
//
//void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float inDeltaTime)
//{
//	Super::NativeTick(MyGeometry, inDeltaTime);
//	
//}

void UTimerWidget::Init()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerWidget::StartTimer, 0.01f, true);
}

void UTimerWidget::StartTimer()
{
	Miliseconds++;

	if (Miliseconds >= 100)
	{
		Miliseconds = 0;
		Seconds++;
	}

	if (Seconds >= 60)
	{
		Seconds = 0;
		Minutes++;
	}

	if (Minutes >= 60)
	{
		Minutes = 0;
		Hours++;
	}

	if (Hours > 0)
	{
		Hour->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	FString FormatMiliSeconds = FString::Printf(TEXT("%02d"), Miliseconds);
	FString FormatSeconds = FString::Printf(TEXT("%02d."), Seconds);
	FString FormatMinutes = FString::Printf(TEXT("%02d:"), Minutes);
	FString FormatHours = FString::Printf(TEXT("%02d:"), Hours);
	MiliSecond->SetText(FText::FromString(FormatMiliSeconds));
	Second->SetText(FText::FromString(FormatSeconds));
	Minute->SetText(FText::FromString(FormatMinutes));
	Hour->SetText(FText::FromString(FormatHours));
}

void UTimerWidget::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

FString UTimerWidget::GetClearTime()
{
	FString FormatClearTime = TEXT("99:99:99.99");
	if (Hours > 0)
	{
		FormatClearTime = FString::Printf(TEXT("%02d:%02d:%02d.%02d"), Hours, Minutes, Seconds, Miliseconds);
	}
	else
	{
		FormatClearTime = FString::Printf(TEXT("%02d:%02d.%02d"), Minutes, Seconds, Miliseconds);
	}

	return FormatClearTime;
}

float UTimerWidget::GetIntClearTime()
{
	float FormatClearTime = 0.f;
	FormatClearTime += (Miliseconds / 100.f);
	FormatClearTime += (Seconds * 1.f);
	FormatClearTime += (Minutes * 60.f);
	FormatClearTime += (Hours * 60.f * 60.f);

	return FormatClearTime;
}