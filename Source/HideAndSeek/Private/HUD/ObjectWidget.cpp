// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ObjectWidget.h"
#include "Components/TextBlock.h"

// 오브젝트 수집 위젯 생성자
void UObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// SwordCount = Cast<UTextBlock>(GetWidgetFromName("SwordGet"));
	if (nullptr == SwordCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: SwordCount is nullptr"));
	}

	// BellCount = Cast<UTextBlock>(GetWidgetFromName("BellGet"));
	if (nullptr == BellCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: BellCount is nullptr"));
	}

	// MirrorCount = Cast<UTextBlock>(GetWidgetFromName("MirrorGet"));
	if (nullptr == MirrorCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: MirrorCount is nullptr"));
	}

	BaseText = "/1";
}

// GameUI에서는 이걸로 오브젝트 수를 관리할 것
void UObjectWidget::SetObjectCount(const unsigned int idx, int32 value)
{
	switch (idx)
	{
		// 검일 경우 검의 개수를 설정함.
		case 1: // Sword
		{
			SetSwordCount(value);
			return;
		}
		// 방울일 경우 방울의 개수를 설정함.
		case 2: // Bell
		{
			SetBellCount(value);
			return;
		}
		// 거울일 경우 거울의 개수를 설정함.
		case 3: // Mirror
		{
			SetMirrorCount(value);
			return;
		}
		// 에러 상황임.
		default: // error
		{
			UE_LOG(LogTemp, Warning, TEXT("Error"));
			return;
		}
	}
}

// 검의 수집 상태를 설정하는 함수.
void UObjectWidget::SetSwordCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	SwordCount->SetText(FText::FromString(curText));
}

// 방울의 수집 상태를 설정하는 함수
void UObjectWidget::SetBellCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	BellCount->SetText(FText::FromString(curText));
}

// 거울의 수집 상태를 설정하는 함수
void UObjectWidget::SetMirrorCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	MirrorCount->SetText(FText::FromString(curText));
}