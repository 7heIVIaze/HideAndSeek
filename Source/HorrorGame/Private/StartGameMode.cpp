// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"
#include "StartWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "HUD/StageSelectWidget.h"
#include "HUD/OptionWidget.h"
#include "UMGPlayerController.h"
#include "HorrorGameGameInstance.h"

AStartGameMode::AStartGameMode()
{
	static ConstructorHelpers::FClassFinder<UStartWidget>StartWidgetClass(TEXT("/Game/Assets/BluePrints/UI/StartWidget_BP"));
	if (StartWidgetClass.Succeeded())
	{
		StartLevelWidgetClass = StartWidgetClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UStageSelectWidget>SelectWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_StageSelectWidget"));
	if (SelectWidgetClass.Succeeded())
	{
		StageSelectWidgetClass = SelectWidgetClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UOptionWidget>OptionWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_OptionWidget"));
	if (OptionWidgetClass.Succeeded())
	{
		OptionSettingWidgetClass = OptionWidgetClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>OpeningMusicAsset(TEXT("/Game/Assets/Sounds/SoundCues/OpeningBGM"));
	if (OpeningMusicAsset.Succeeded())
	{
		OpeningCue = OpeningMusicAsset.Object;
	}

	PlayerControllerClass = AUMGPlayerController::StaticClass();

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetAutoActivate(false);
}

void AStartGameMode::BeginPlay()
{
	Super::BeginPlay();

	// UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
	/*FString CurrentLevelName = GetWorld()->GetMapName();
	UE_LOG(LogTemp, Warning, TEXT("StartGameMode: CurrentLevelName = %s"), *CurrentLevelName);
	if (CurrentLevelName.Contains(TEXT("Start")))
	{
		if (IsValid(StartLevelWidgetClass))
		{
			StartWidget = Cast<UStartWidget>(CreateWidget(GetWorld(), StartLevelWidgetClass));
			if (IsValid(StartWidget))
			{
				StartWidget->AddToViewport();
			}
		}
	}
	else if (CurrentLevelName.Contains(TEXT("SelectStage")))
	{
		if (IsValid(StageSelectWidgetClass))
		{
			StageWidget = Cast<UStageSelectWidget>(CreateWidget(GetWorld(), StageSelectWidgetClass));
			if (IsValid(StageWidget))
			{
				StageWidget->AddToViewport();
			}
		}
	}*/
	// GameInstance->ChangeWidget(StartLevelWidgetClass);
	ChangeWidget(WidgetType::StartWidget);

	AudioComponent->SetSound(OpeningCue);
	AudioComponent->Play();
}

void AStartGameMode::ChangeWidget(WidgetType widgetNumber)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	switch (widgetNumber)
	{
	case WidgetType::StartWidget:
		if (StartLevelWidgetClass != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartLevelWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;
	case WidgetType::LevelSelectWidget:
		if (StageSelectWidgetClass != nullptr)
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StageSelectWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;
	case WidgetType::OptionSettingWidget:
		if (IsValid(OptionSettingWidgetClass))
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), OptionSettingWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
	}
	
}