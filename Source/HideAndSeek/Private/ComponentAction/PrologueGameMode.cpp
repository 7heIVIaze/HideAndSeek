// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

	static ConstructorHelpers::FObjectFinder<USoundCue>DiedCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/DiedCue"));
	if (DiedCueAsset.Succeeded())
	{
		DiedCue = DiedCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>ClearCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/ClearCue"));
	if (ClearCueAsset.Succeeded())
	{
		ClearCue = ClearCueAsset.Object;
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
	
	// 오디오 컴포넌트의 기본 소리를 Main Cue로 설정하고 재생함.
	AudioComponent->SetSound(MainCue);
	AudioComponent->Play();

	// 적이 근처에 있을 때의 소리도 설정하고 자동 재생은 off함.
	NervousAudioComponent->SetSound(NervousCue);
	NervousAudioComponent->SetAutoActivate(false);
}

void APrologueGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 근처에 적이 있을 때 BGM을 변경할 함수.
void APrologueGameMode::PlayNervousBackGroundMusic()
{
	// 메인 BGM을 페이드아웃 시키고, 긴장 BGM을 페이드 인 시킴.
	AudioComponent->FadeOut(0.75f, 0.f);
	NervousAudioComponent->FadeIn(1.0f);
}

// 근처에 적이 없을 때 BGM을 변경할 함수.
void APrologueGameMode::StopNervousBackGroundMusic()
{
	// 긴장 BGM을 페이드아웃 시키고, 메인 BGM을 페이드 인 시킴.
	NervousAudioComponent->FadeOut(0.75f, 0.f);

	AudioComponent->FadeIn(0.75f);
}

// 사망했을 때 BGM을 변경할 함수.
void APrologueGameMode::PlayDiedBackGroundMusic()
{
	// 모든 BGM을 끄고, 메인 BGM을 Died Cue로 변경한 후 재생시킴.
	NervousAudioComponent->Stop();
	AudioComponent->Stop();
	AudioComponent->SetSound(DiedCue);
	AudioComponent->Play();
}

// 클리어 시 BGM을 변경할 함수.
void APrologueGameMode::PlayClearBackGroundMusic()
{
	// 모든 BGM을 끄고, 메인 BGM을 Clear Cue로 변경한 후 재생시킴.
	NervousAudioComponent->Stop();
	AudioComponent->Stop();
	AudioComponent->SetSound(ClearCue);
	AudioComponent->Play();
}