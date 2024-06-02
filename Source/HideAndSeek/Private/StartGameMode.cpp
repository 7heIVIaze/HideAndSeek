// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "StartGameMode.h"
#include "StartWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "HUD/StageSelectWidget.h"
#include "HUD/OptionWidget.h"
#include "HUD/GraphicsOption.h"
#include "HUD/GameSettingWidget.h"
#include "HUD/ArchiveWidget.h"
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

	static ConstructorHelpers::FClassFinder<UGameSettingWidget>GameWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_GameplaySettingWidget"));
	if (GameWidgetClass.Succeeded())
	{
		GameSettingWidgetClass = GameWidgetClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UArchiveWidget>BP_ArchiveWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_ArchiveWidget"));
	if (BP_ArchiveWidgetClass.Succeeded())
	{
		ArchiveWidgetClass = BP_ArchiveWidgetClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UGraphicsOption>GraphicWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_GraphicOptionWidget"));
	if (GraphicWidgetClass.Succeeded())
	{
		GraphicsSettingWidgetClass = GraphicWidgetClass.Class;
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
		break;
	case WidgetType::GameplaySettingWidget:
		if (IsValid(GameSettingWidgetClass))
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameSettingWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;
	case WidgetType::GraphicsSettingWidget:
		if (IsValid(GraphicsSettingWidgetClass))
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GraphicsSettingWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;
	case WidgetType::ArchiveWidget:
		if (IsValid(ArchiveWidgetClass))
		{
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ArchiveWidgetClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
			}
		}
		break;
	}
}

void AStartGameMode::SetCurrentWidget(UUserWidget* inWidgetObject)
{
	CurrentWidget = inWidgetObject;
}