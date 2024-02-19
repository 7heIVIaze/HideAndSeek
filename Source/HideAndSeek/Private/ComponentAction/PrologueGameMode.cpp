// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ComponentAction/PrologueGameMode.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.h"
#include "DiedWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "HorrorGamePlayerController.h"
#include "UObject/ConstructorHelpers.h"

APrologueGameMode::APrologueGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	//DefaultPawnClass = AHorrorGameCharacter::StaticClass();
	PlayerControllerClass = AHorrorGamePlayerController::StaticClass();

	// set default hud class to our Blueprinted HUD
	//static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClassFinder(TEXT("/Game/Assets/BluePrints/UI/Player_HUD"));
	//HUDClass = PlayerHUDClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UGameUI>MainWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BasicInterface"));
	if (MainWidgetClass.Succeeded())
	{
		LevelWidgetClass = MainWidgetClass.Class;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue>MainMusicAsset(TEXT("/Game/Assets/Sounds/SoundCues/PrologueBGM"));
	if (MainMusicAsset.Succeeded())
	{
		MainCue = MainMusicAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>NervousCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/NervousCue"));
	if (NervousCueAsset.Succeeded())
	{
		NervousCue = NervousCueAsset.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetAutoActivate(false);

	NervousAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NervAudioComp"));
	NervousAudioComponent->SetAutoActivate(false);
}

void APrologueGameMode::BeginPlay()
{
	Super::BeginPlay();
	//
	//if (IsValid(LevelWidgetClass))
	//{
	//	MainWidget = Cast<UGameUI>(CreateWidget(GetWorld(), LevelWidgetClass));
	//	if (IsValid(MainWidget))
	//	{
	//		MainWidget->AddToViewport();
	//	}
	//}

	AudioComponent->SetSound(MainCue);
	AudioComponent->Play();

	NervousAudioComponent->SetSound(NervousCue);
	NervousAudioComponent->SetAutoActivate(false);
}

void APrologueGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APrologueGameMode::PlayNervousBackGroundMusic()
{
	AudioComponent->FadeOut(0.75f, 0.f);
	NervousAudioComponent->FadeIn(1.0f);
}

void APrologueGameMode::StopNervousBackGroundMusic()
{
	NervousAudioComponent->FadeOut(0.75f, 0.f);

	AudioComponent->FadeIn(0.75f);
}