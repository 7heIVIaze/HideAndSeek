// Fill out your copyright notice in the Description page of Project Settings.

#include "StartWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "HorrorGameGameInstance.h"
#include "StartGameMode.h"
#include "HUD/StageSelectWidget.h"
// #include "HorrorGameGameInstance.h"

//UStartWidget::UStartWidget(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	static ConstructorHelpers::FClassFinder<UUserWidget>SelectWidget(TEXT("/Game/Assets/BluePrints/UI/BP_StageSelectWidget"));
//	if (SelectWidget.Succeeded())
//	{
//		WidgetClass = SelectWidget.Class;
//	}
//}

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
		// StartButton->SetKeyboardFocus();
		StartButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickStartButton);
		StartButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredStartButton);
	}

	if (nullptr != OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickOptionButton);
		OptionButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredOptionButton);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Option Button is not null")));
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Option Button is null")));
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredExitButton);
	}

	if (nullptr != Menu)
	{
		Menu->GetAllChildren();
	}

	UpdateButtonSlate();
}

void UStartWidget::OnClickStartButton()
{
	//UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
	//if (IsValid(GameInstance))
	//{
	//	UGameplayStatics::OpenLevel(GetWorld(), GameInstance->LevelArray[]);
	//}
	/*FString levelName = TEXT("/Game/Levels/GameLevel/SelectStage");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);*/
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//auto SelectWidget = LoadClass<UUserWidget>(NULL, TEXT("/Game/Assets/BluePrints/UI/BP_StageSelectWidget"));
	/*if (WidgetClass == nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Select Widget NULL")));
	}
	if (WidgetClass)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Select Widget On")));
	}*/
	GameMode->ChangeWidget(WidgetType::LevelSelectWidget);

	//UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
	//if (GameInstance)
	//{
	//	/*GameInstace->ShowStageSelectWidget(true);
	//	this->RemoveFromViewport();*/
	//	GameInstance->ChangeWidget(WidgetClass);
	//}
}

void UStartWidget::OnClickOptionButton()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Option Select")));
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

void UStartWidget::OnClickExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UStartWidget::OnHoveredStartButton()
{
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
}

void UStartWidget::OnHoveredOptionButton()
{
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

void UStartWidget::OnHoveredExitButton()
{
	MenuNavigationIndex = 2;
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
		break;
	case 1:
		StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		OptionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		break;
	case 2:
		StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
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
			OnClickOptionButton();
			break;
		case 2:
			OnClickExitButton();
			break;
		}
	}
	else
	{
		if (KeyType == "S" || KeyType == "Down")
		{
			MenuNavigationIndex++;
			if (MenuNavigationIndex >= MenuNumber)
			{
				MenuNavigationIndex = 0;
			}
		}
		else if (KeyType == "W" || KeyType == "Up")
		{
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