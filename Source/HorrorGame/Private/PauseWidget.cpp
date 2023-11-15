// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseWidget.h"
#include "Components/Button.h"
#include "HorrorGamePlayerController.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeBtn")));
	RetryButton = Cast<UButton>(GetWidgetFromName(TEXT("RetryBtn")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitBtn")));

	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickResumeButton);
	}

	if (nullptr != RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickRetryButton);
	}

	if (nullptr != QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickQuitButton);
	}
	UpdateButtonSlate();
}

void UPauseWidget::OnClickResumeButton()
{
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());

	RemoveFromParent();

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);
}

void UPauseWidget::OnClickRetryButton()
{
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());

	PlayerController->RestartLevel();
}

void UPauseWidget::OnClickQuitButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UPauseWidget::OnHoveredResumeButton()
{
	MenuNavIndex = 0;
	UpdateButtonSlate();
}

void UPauseWidget::OnHoveredRetryButton()
{
	MenuNavIndex = 1;
	UpdateButtonSlate();
}

void UPauseWidget::OnHoveredQuitButton()
{
	MenuNavIndex = 2;
	UpdateButtonSlate();
}

void UPauseWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
	case 0:
		ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	case 1:
		ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	case 2:
		ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		break;
	default:
		ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

FReply UPauseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey keyCode = InKeyEvent.GetKey();
	FString keyType = keyCode.ToString();

	if (keyType == "Enter")
	{
		switch (MenuNavIndex)
		{
		case 0:
			OnClickResumeButton();
			break;
		case 1:
			OnClickRetryButton();
			break;
		case 2:
			OnClickQuitButton();
			break;
		}
	}
	else
	{
		if (keyType == "S" || keyType == "Down")
		{
			MenuNavIndex++;
			if (MenuNavIndex >= MenuNum)
			{
				MenuNavIndex = 0;
			}
		}
		else if (keyType == "W" || keyType == "Up")
		{
			MenuNavIndex--;
			if (MenuNavIndex < 0)
			{
				MenuNavIndex = MenuNum - 1;
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}