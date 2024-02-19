// Fill out your copyright notice in the Description page of Project Settings.

#include "DiedWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HorrorGamePlayerController.h"
#include "Sound/SoundCue.h"

void UDiedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartBtn")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));

	if (nullptr != RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickRestartButton);
		RestartButton->OnHovered.AddDynamic(this, &UDiedWidget::OnHoveredRestartButton);
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UDiedWidget::OnHoveredExitButton);
	}

	UpdateButtonSlate();
}

void UDiedWidget::OnClickRestartButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto playerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());
	playerController->RestartLevel();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UDiedWidget::OnClickExitButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Game/Levels/Start"));
}

void UDiedWidget::OnHoveredRestartButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 0;
	UpdateButtonSlate();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UDiedWidget::OnHoveredExitButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 1;
	UpdateButtonSlate();
}

void UDiedWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
	case 0:
		RestartButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	case 1:
		RestartButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		break;
	}
}

void UDiedWidget::SetArchiveGetText(FText inText)
{
	if (ArchiveGetText == nullptr) return;
	ArchiveGetText->SetText(inText);
}

FReply UDiedWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey keyCode = InKeyEvent.GetKey();
	FString keyType = keyCode.ToString();

	if (keyType == "Enter")
	{
		switch (MenuNavIndex)
		{
		case 0:
			OnClickRestartButton();
			break;
		case 1:
			OnClickExitButton();
			break;
		}
	}
	else
	{
		if (keyType == "S" || keyType == "Down")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			MenuNavIndex++;
			if (MenuNavIndex >= MenuNum)
			{
				MenuNavIndex = 0;
			}
		}
		else if (keyType == "W" || keyType == "Up")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
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