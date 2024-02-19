// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "PauseWidget.h"
#include "Components/Button.h"
#include "HorrorGamePlayerController.h"
#include "Sound/SoundCue.h"
#include "HUD/ArchiveWidget.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeBtn")));
	RetryButton = Cast<UButton>(GetWidgetFromName(TEXT("RetryBtn")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitBtn")));

	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickResumeButton);
		ResumeButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredResumeButton);
	}

	if (nullptr != RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickRetryButton);
		RetryButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredRetryButton);
	}

	if (nullptr != QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickQuitButton);
		QuitButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredQuitButton);
	}

	ArchiveButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickArchiveButton);
	ArchiveButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredArchiveButton);
	UpdateButtonSlate();
}

void UPauseWidget::OnClickResumeButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());

	RemoveFromParent();

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);
}

void UPauseWidget::OnClickArchiveButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	UArchiveWidget* NewWidget = CreateWidget<UArchiveWidget>(GetWorld(), ArchiveWidget);
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = false;

	RemoveFromParent();
}

void UPauseWidget::OnClickRetryButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());

	PlayerController->RestartLevel();
}

void UPauseWidget::OnClickQuitButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void UPauseWidget::OnHoveredResumeButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 0;
	UpdateButtonSlate();
}

void UPauseWidget::OnHoveredArchiveButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 1;
	UpdateButtonSlate();
}


void UPauseWidget::OnHoveredRetryButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 2;
	UpdateButtonSlate();
}

void UPauseWidget::OnHoveredQuitButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavIndex = 3;
	UpdateButtonSlate();
}

void UPauseWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
	case 0:
		ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 1:
		ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 2:
		ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 3:
		ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	default:
		ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
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
			OnClickArchiveButton();
			break;
		case 2:
			OnClickRetryButton();
			break;
		case 3:
			OnClickQuitButton();
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