// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/TimerWidget.h"
#include "Components/TextBlock.h"

// 게임 플레이 타이머 위젯 생성자.
void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 기본적으로 분, 초만 보이게 하고, 시간은 안 보이게 설정함.
	Hour->SetVisibility(ESlateVisibility::Collapsed);

}
//
//void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float inDeltaTime)
//{
//	Super::NativeTick(MyGeometry, inDeltaTime);
//	
//}

// 타이머 시작 함수.
void UTimerWidget::Init()
{
	// 타이머 핸들을 이용해 게임 플레이 시간을 측정함.
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerWidget::StartTimer, 0.01f, true);
}

// 게임 플레이 시간을 측정하는 함수.
void UTimerWidget::StartTimer()
{
	// 타이머 핸들을 이용해 0.01초마다 반복되기 때문에 밀리 세컨드를 계속 1씩 더함.
	Miliseconds++;

	// 밀리 세컨드가 100 밀리 세컨드가 되면, 1초씩 증가시킴.
	if (Miliseconds >= 100)
	{
		Miliseconds = 0;
		Seconds++;
	}

	// 초가 60초가 되면, 1분씩 증가시킴.
	if (Seconds >= 60)
	{
		Seconds = 0;
		Minutes++;
	}

	// 분이 60분이 되면, 1시간씩 증가시킴.
	if (Minutes >= 60)
	{
		Minutes = 0;
		Hours++;
	}

	// 시간이 1시간 이상으로 될 경우, 시간 텍스트를 보이게 설정함.
	if (Hours > 0)
	{
		Hour->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	// 측정한 시간을 텍스트에 적용시키기 위한 로직임.
	FString FormatMiliSeconds = FString::Printf(TEXT("%02d"), Miliseconds);
	FString FormatSeconds = FString::Printf(TEXT("%02d."), Seconds);
	FString FormatMinutes = FString::Printf(TEXT("%02d:"), Minutes);
	FString FormatHours = FString::Printf(TEXT("%02d:"), Hours);
	MiliSecond->SetText(FText::FromString(FormatMiliSeconds));
	Second->SetText(FText::FromString(FormatSeconds));
	Minute->SetText(FText::FromString(FormatMinutes));
	Hour->SetText(FText::FromString(FormatHours));
}

// 타이머를 정지하는 함수로, 작동한 타이머 핸들을 제거함.
void UTimerWidget::StopTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// String 타입으로 클리어 시간을 가져올 함수
FString UTimerWidget::GetClearTime()
{
	// 1 시간 이상 플레이 한 경우, 시간도 붙이고, 아닐 경우, 분까지만 붙여서 리턴함.
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

// float 타입으로 클리어 시간을 가져올 함수.(함수 명에 int는 숫자 타입이라는 것으로 표기하기 위함임...)
float UTimerWidget::GetIntClearTime()
{
	// 클리어 타임을 초의 형태로 변환하여 리턴함.
	float FormatClearTime = 0.f;
	FormatClearTime += (Miliseconds / 100.f);
	FormatClearTime += (Seconds * 1.f);
	FormatClearTime += (Minutes * 60.f);
	FormatClearTime += (Hours * 60.f * 60.f);

	return FormatClearTime;
}