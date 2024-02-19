// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/ClearWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HorrorGameGameInstance.h"
#include "Sound/SoundCue.h"

void UClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	NextStageButton = Cast<UButton>(GetWidgetFromName(TEXT("NextStage")));
	BackToMainButton = Cast<UButton>(GetWidgetFromName(TEXT("BackToMain")));
	CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());

	if (ClearTime != nullptr)
	{
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			//FText ClearedTime = GameInstance->GetClearTime(ClearedChapter);
			ClearTime->SetText(ClearedTime);
		}
	}

	if (BackToMainButton)
	{
		BackToMainButton->OnClicked.AddDynamic(this, &UClearWidget::OnClickBackToMain);
		BackToMainButton->OnHovered.AddDynamic(this, &UClearWidget::OnHoveredBackToMain);
	}

	if (CurrentLevel != "Level1")
	{
		if (NextStageButton)
		{
			NextStageButton->OnClicked.AddDynamic(this, &UClearWidget::OnClickNextStage);
			NextStageButton->OnHovered.AddDynamic(this, &UClearWidget::OnHoveredNextStage);
			MenuNumber = 2;
			bHaveNextLevel = true;
		}
	}
	else
	{
		NextStageButton->SetVisibility(ESlateVisibility::Hidden);
		MenuNumber = 1;
		bHaveNextLevel = false;
	}

	UpdateButtonSlate();
}

void UClearWidget::OnClickNextStage()
{
	if (CurrentLevel == "Prologue")
	{
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		FString NextLevelName = TEXT("/Game/Levels/GameLevel/Level1");
		UGameplayStatics::OpenLevel(GetWorld(), *NextLevelName);
	}
}

void UClearWidget::OnClickBackToMain()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	FString levelName = TEXT("/Game/Levels/GameLevel/Start");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UClearWidget::OnHoveredNextStage()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
	
}

void UClearWidget::OnHoveredBackToMain()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

void UClearWidget::SetClearTime(FString inClearTime)
{
	ClearTime->SetText(FText::FromString(inClearTime));
}

void UClearWidget::UpdateButtonSlate()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SelectWidget: MenuNavigation: %d"), MenuNavigationIndex));*/
	if (bHaveNextLevel)
	{
		switch (MenuNavigationIndex)
		{
		case 0:
			NextStageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BackToMainButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			NextStageButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackToMainButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
	}
	else
	{
		NextStageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

FReply UClearWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("KeyType: %s"), *KeyType));*/
	if (KeyType == "Enter")
	{
		if (bHaveNextLevel)
		{
			switch (MenuNavigationIndex)
			{
			case 0:
				OnClickNextStage();
				break;
			case 1:
				OnClickBackToMain();
				break;
			}
		}
		else
		{
			OnClickBackToMain();
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