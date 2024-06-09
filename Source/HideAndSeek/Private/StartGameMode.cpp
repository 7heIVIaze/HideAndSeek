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
	// 필요한 위젯 클래스를 블루프린트 클래스로부터 가져옴.
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

	// BGM도 가져옴.
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

	// 시작 위젯을 출력하고 BGM을 재생함.
	ChangeWidget(WidgetType::StartWidget);

	AudioComponent->SetSound(OpeningCue);
	AudioComponent->Play();
}

// 위젯을 바꾸는 함수.
void AStartGameMode::ChangeWidget(WidgetType widgetNumber)
{
	// 현재 뷰포트에 출력된 위젯이 있다면, 뷰포트에서 제거하고, null 포인터로 설정함.
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	switch (widgetNumber)
	{
		// 시작 위젯인 경우
		case WidgetType::StartWidget:
		{
			if (StartLevelWidgetClass != nullptr)
			{
				// 시작 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StartLevelWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
		// 챕터 선택 위젯인 경우
		case WidgetType::LevelSelectWidget:
		{
			if (StageSelectWidgetClass != nullptr)
			{
				// 챕터 선택 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), StageSelectWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
		// 옵션 설정 위젯인 경우
		case WidgetType::OptionSettingWidget:
		{
			if (IsValid(OptionSettingWidgetClass))
			{
				// 옵션 설정 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), OptionSettingWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
		// 게임 플레이 설정 위젯인 경우
		case WidgetType::GameplaySettingWidget:
		{
			if (IsValid(GameSettingWidgetClass))
			{
				// 게임 플레이 설정 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GameSettingWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
		// 그래픽 설정 위젯인 경우
		case WidgetType::GraphicsSettingWidget:
		{
			if (IsValid(GraphicsSettingWidgetClass))
			{
				// 그래픽 설정 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), GraphicsSettingWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
		// 문서 보관함 위젯인 경우
		case WidgetType::ArchiveWidget:
		{
			if (IsValid(ArchiveWidgetClass))
			{
				// 문서 보관함 위젯을 생성하고 뷰포트에 출력함.
				CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), ArchiveWidgetClass);
				if (CurrentWidget != nullptr)
				{
					CurrentWidget->AddToViewport();
				}
			}
			return;
		}
	}
}

void AStartGameMode::SetCurrentWidget(UUserWidget* inWidgetObject)
{
	CurrentWidget = inWidgetObject;
}