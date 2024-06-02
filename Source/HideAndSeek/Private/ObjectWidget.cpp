// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "ObjectWidget.h"
#include "Components/TextBlock.h"

void UObjectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SwordCount = Cast<UTextBlock>(GetWidgetFromName("SwordGet"));
	if (nullptr == SwordCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: SwordCount is nullptr"));
	}

	BellCount = Cast<UTextBlock>(GetWidgetFromName("BellGet"));
	if (nullptr == BellCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: BellCount is nullptr"));
	}

	MirrorCount = Cast<UTextBlock>(GetWidgetFromName("MirrorGet"));
	if (nullptr == MirrorCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjectWidget: MirrorCount is nullptr"));
	}

	BaseText = "/1";
}

void UObjectWidget::SetObjectCount(const unsigned int idx, int32 value) // GameUI에서는 이걸로 관리할 것
{
	switch (idx)
	{
	case 1: // Sword
		SetSwordCount(value);
		break;
	case 2: // Bell
		SetBellCount(value);
		break;
	case 3: // Mirror
		SetMirrorCount(value);
		break;
	default: // error
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString::Printf(TEXT("Error")));
		break;
	}
}

void UObjectWidget::SetSwordCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	SwordCount->SetText(FText::FromString(curText));
}

void UObjectWidget::SetBellCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	BellCount->SetText(FText::FromString(curText));
}

void UObjectWidget::SetMirrorCount(int32 value)
{
	FString curText = FString::FromInt(value) + BaseText;

	MirrorCount->SetText(FText::FromString(curText));
}