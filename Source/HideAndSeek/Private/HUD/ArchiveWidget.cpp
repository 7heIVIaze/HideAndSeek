// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/ArchiveWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "Sound/SoundCue.h"
#include "HUD/ArchiveDocsWidget.h"
#include "HUD/HintWidget.h"
#include "PauseWidget.h"

void UArchiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();
	/*bIsFocusable = true;
	bIsEnabled = true;*/

	Item->OnClicked.AddDynamic(this, &UArchiveWidget::OnItemButtonClick);
	Item->OnHovered.AddDynamic(this, &UArchiveWidget::OnItemButtonHovered);

	Document->OnClicked.AddDynamic(this, &UArchiveWidget::OnDocumentButtonClick);
	Document->OnHovered.AddDynamic(this, &UArchiveWidget::OnDocumentButtonHovered);

	Letter->OnClicked.AddDynamic(this, &UArchiveWidget::OnLetterButtonClick);
	Letter->OnHovered.AddDynamic(this, &UArchiveWidget::OnLetterButtonHovered);

	Hint->OnClicked.AddDynamic(this, &UArchiveWidget::OnHintButtonClick);
	Hint->OnHovered.AddDynamic(this, &UArchiveWidget::OnHintButtonHovered);

	Exit->OnClicked.AddDynamic(this, &UArchiveWidget::OnExitButtonClick);
	Exit->OnHovered.AddDynamic(this, &UArchiveWidget::OnExitButtonHovered);

	NavIndex = 5;
	CurrentIndex = 0;

	//bIsStartGameMode = true;

	if (bIsStartGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("From Start Game Mode"));
	}

	UpdateButtonSlate();
}

void UArchiveWidget::OnExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->ChangeWidget(WidgetType::StartWidget);

	}
	else
	{
		UPauseWidget* NewWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidget);
		NewWidget->AddToViewport();

		RemoveFromParent();
	}
}

void UArchiveWidget::OnItemButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);

	//this->bIsFocusable = false;
	NewWidget->ArchiveMode = EArchiveModes::AM_Item;
	//NewWidget->ParentWidget = this;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

void UArchiveWidget::OnDocumentButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);
	
	//this->bIsFocusable = false;
	NewWidget->ArchiveMode = EArchiveModes::AM_Document;
	//NewWidget->ParentWidget = this;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

void UArchiveWidget::OnLetterButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);

	//this->bIsFocusable = false;
	NewWidget->ArchiveMode = EArchiveModes::AM_Letter;
	//NewWidget->ParentWidget = this;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

void UArchiveWidget::OnHintButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UHintWidget* NewWidget = CreateWidget<UHintWidget>(GetWorld(), HintWidget);
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	//this->bIsFocusable = false;
	//NewWidget->FromWidget = this;
	NewWidget->bIsStartGameMode = bIsStartGameMode;
	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

void UArchiveWidget::OnExitButtonHovered()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = 4;
	UpdateButtonSlate();
}

void UArchiveWidget::OnItemButtonHovered()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = 0;
	UpdateButtonSlate();
}

void UArchiveWidget::OnDocumentButtonHovered()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = 1;
	UpdateButtonSlate();
}

void UArchiveWidget::OnLetterButtonHovered()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = 2;
	UpdateButtonSlate();
}

void UArchiveWidget::OnHintButtonHovered()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = 3;
	UpdateButtonSlate();
}

void UArchiveWidget::UpdateButtonSlate()
{
	switch (CurrentIndex)
	{
	case 0:
		Item->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 1:
		Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Document->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 2:
		Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Letter->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 3:
		Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Hint->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 4:
		Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		break;
	}
}

FReply UArchiveWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		switch (CurrentIndex)
		{
		case 0:
			OnItemButtonClick();
			break;
		case 1:
			OnDocumentButtonClick();
			break;
		case 2:
			OnLetterButtonClick();
			break;
		case 3:
			OnHintButtonClick();
			break;
		case 4:
			OnExitButtonClick();
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
			CurrentIndex++;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex = 0;
			}
		}
		else if (KeyType == "W" || KeyType == "Up")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			CurrentIndex--;
			if (CurrentIndex < 0)
			{
				CurrentIndex = NavIndex - 1;
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}