// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "StartWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "HorrorGameGameInstance.h"
#include "StartGameMode.h"
#include "HUD/StageSelectWidget.h"
#include "Sound/SoundCue.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartBtn")));
	OptionButton = Cast<UButton>(GetWidgetFromName(TEXT("OptionBtn")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));
	Menu = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MenuPanel")));

	if (nullptr != StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickStartButton);
		StartButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredStartButton);
	}

	if (nullptr != OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickOptionButton);
		OptionButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredOptionButton);
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredExitButton);
	}

	if (nullptr != ArchiveBtn)
	{
		ArchiveBtn->OnClicked.AddDynamic(this, &UStartWidget::OnClickArchiveButton);
		ArchiveBtn->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredArchiveButton);
	}

	if (nullptr != Menu)
	{
		MenuNumber = Menu->GetChildrenCount();
	}

	UpdateButtonSlate();

	/*UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		

		TArray<FString> ClearedTime = GameInstance->GetAllClearedTime();
	
	}*/
}

void UStartWidget::OnClickStartButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::LevelSelectWidget);

}

void UStartWidget::OnClickOptionButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

void UStartWidget::OnClickExitButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UStartWidget::OnClickArchiveButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::ArchiveWidget);
}

void UStartWidget::OnHoveredStartButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
}

void UStartWidget::OnHoveredOptionButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 2;
	UpdateButtonSlate();
}

void UStartWidget::OnHoveredExitButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 3;
	UpdateButtonSlate();
}

void UStartWidget::OnHoveredArchiveButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

void UStartWidget::UpdateButtonSlate()
{
	switch(MenuNavigationIndex)
	{
	case 0:
		StartButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 1:
		StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 2:
		StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		OptionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 3:
		StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	}
}

FReply UStartWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		switch (MenuNavigationIndex)
		{
		case 0:
			OnClickStartButton();
			break;
		case 1:
			OnClickArchiveButton();
			break;
		case 2:
			OnClickOptionButton();
			break;
		case 3:
			OnClickExitButton();
			break;
		}
	}
	else
	{
		if (KeyType == "S" || KeyType == "Down")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			MenuNavigationIndex++;
			if (MenuNavigationIndex >= MenuNumber)
			{
				MenuNavigationIndex = 0;
			}
		}
		else if (KeyType == "W" || KeyType == "Up")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			MenuNavigationIndex--;
			if (MenuNavigationIndex < 0)
			{
				MenuNavigationIndex = MenuNumber - 1;
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}