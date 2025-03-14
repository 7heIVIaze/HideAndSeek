// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/MessageManagerWidget.h"
#include "HUD/MessageWidget.h"
#include "Components/VerticalBox.h"

void UMessageManagerWidget::AddMessage(FText Message, float Duration)
{
	if (!MessageListBox)
	{
		return;
	}

	// Create message widget.
	UMessageWidget* NewMessage = CreateWidget<UMessageWidget>(GetWorld(), MessageWidget);
	if (!NewMessage)
	{
		return;
	}

	if (MessageQueue.Num() > 6)
	{
		CleanupMessages();
	}

	MessageListBox->AddChildToVerticalBox(NewMessage);
	NewMessage->SetMessage(Message, Duration);
	NewMessage->SetParentWidget(this);
	MessageQueue.Add(NewMessage);
	MessageCount++;

	UE_LOG(LogTemp, Warning, TEXT("Message Count: %d"), MessageCount);
}

void UMessageManagerWidget::CleanupMessages()
{
	if (MessageQueue.Num() == 0)
	{
		return;
	}

	// Remove the oldest message.
	UMessageWidget* OldMessage = MessageQueue[0];
	MessageQueue.RemoveAt(0);
	OldMessage->RemoveWidget();
}