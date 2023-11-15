// Fill out your copyright notice in the Description page of Project Settings.

#include "DiedWidget.h"
#include "Components/Button.h"
#include "HorrorGamePlayerController.h"

void UDiedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartBtn")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));

	if (nullptr != RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickRestartButton);
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickExitButton);
	}
}

void UDiedWidget::OnClickRestartButton()
{
	auto playerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());
	playerController->RestartLevel();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UDiedWidget::OnClickExitButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Game/Levels/Start"));
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

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *keyType));

	return reply;
}