// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/StageSelectWidget.h"
#include "Components/Button.h"
#include "HorrorGameGameInstance.h"
#include "StartGameMode.h"
#include "Components/Image.h"

//UStageSelectWidget::UStageSelectWidget(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	static ConstructorHelpers::FClassFinder<UUserWidget>StartWidget(TEXT("/Game/Assets/BluePrints/UI/StartWidget_BP"));
//	if (StartWidget.Succeeded())
//	{
//		WidgetClass = StartWidget.Class;
//	}
//}

void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();
	/*FInputKeyBinding DownKey(
		FKey("W"),
		EInputEvent::IE_Pressed,
		this,
		&UStageSelectWidget::UpdateButtonSlate
	);*/

	ChapOneButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapOneBtn")));
	ChapTwoButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapTwoBtn")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	LevelImg = Cast<UImage>(GetWidgetFromName(TEXT("StageSampleImg")));
	//GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());

	if (nullptr != ChapOneButton)
	{
		ChapOneButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapOneButton);
		ChapOneButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapOneButton);
	}

	if (nullptr != ChapTwoButton)
	{
		ChapTwoButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapTwoButton);
		ChapTwoButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapTwoButton);
	}

	if (nullptr != BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredBackButton);
	}

	UpdateButtonSlate();
}

void UStageSelectWidget::OnClickChapOneButton()
{
	FString levelName = TEXT("/Game/Levels/GameLevel/Prologue");
	//if(GameInstance)
	//	GameInstance->StopSound();
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UStageSelectWidget::OnClickChapTwoButton()
{
	FString levelName = TEXT("/Game/Levels/GameLevel/Level1");
	/*if(GameInstance)
		GameInstance->StopSound();*/
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UStageSelectWidget::OnClickBackButton()
{
	/*UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		GameInstance->ChangeWidget(WidgetClass);
	}*/
	/*FString levelName = TEXT("/Game/Levels/GameLevel/Start");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);*/
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	//auto StartWidget = LoadClass<UStageSelectWidget>(NULL, TEXT("/Game/Assets/BluePrints/UI/StartWidget_BP"));
	/*if (WidgetClass == nullptr)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Start Widget NULL")));
	}
	if (WidgetClass)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Start Widget On")));
	}*/
	GameMode->ChangeWidget(WidgetType::StartWidget);
}

void UStageSelectWidget::OnHoveredChapOneButton()
{
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
}

void UStageSelectWidget::OnHoveredChapTwoButton()
{
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

void UStageSelectWidget::OnHoveredBackButton()
{
	MenuNavigationIndex = 2;
	UpdateButtonSlate();
}

void UStageSelectWidget::UpdateButtonSlate()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SelectWidget: MenuNavigation: %d"), MenuNavigationIndex));*/
	switch (MenuNavigationIndex)
	{
	case 0:
		ChapOneButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		break;
	case 1:
		ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		break;
	case 2:
		ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		break;
	}
}

FReply UStageSelectWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();
	
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("KeyType: %s"), *KeyType));*/
	if (KeyType == "Enter")
	{
		switch (MenuNavigationIndex)
		{
		case 0:
			OnClickChapOneButton();
			break;
		case 1:
			OnClickChapTwoButton();
			break;
		case 2:
			OnClickBackButton();
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