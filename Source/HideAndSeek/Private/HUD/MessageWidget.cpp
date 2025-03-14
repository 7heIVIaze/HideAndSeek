// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/MessageWidget.h"
#include "HUD/MessageManagerWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsMessageOn = false;
	Timer = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Message Widget Construct"));

	FadeoutAnimEvent.BindDynamic(this, &UMessageWidget::FadeOutFinished);
	BindToAnimationFinished(FadeoutAnim, FadeoutAnimEvent);
}

void UMessageWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (bIsMessageOn)
	{
		Timer += DeltaTime;

		if (Timer >= MessageDuration)
		{
			Manager->CleanupMessages();
			Timer = 0.0f;
			bIsMessageOn = false;
		}
	}
}

void UMessageWidget::SetMessage(FText Message, float Duration)
{
	if (Message.IsEmptyOrWhitespace())
	{
		UE_LOG(LogTemp, Warning, TEXT("Message is empty"));
		return;
	}

	if (!MessageText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Message Text is null"));
		return;
	}
	MessageText->SetText(Message);
	MessageDuration = Duration;
	bIsMessageOn = true;
}

void UMessageWidget::RemoveWidget()
{
	StartFadeout();
}

void UMessageWidget::SetParentWidget(class UMessageManagerWidget* ManagerWidget)
{
	Manager = ManagerWidget;
}

void UMessageWidget::StartFadeout()
{
	PlayAnimation(FadeoutAnim);
}

void UMessageWidget::FadeOutFinished()
{
	RemoveFromParent();
}