// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LevelManager/HorrorGameGameMode.h"
#include "Player/HorrorGameCharacter.h"
#include "Blueprint/UserWidget.h"
#include "HUD/GameUI.h"
#include "HUD/DiedWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Player/HorrorGamePlayerController.h"
#include "AI/CreatureAI.h"
#include "UObject/ConstructorHelpers.h"

AHorrorGameGameMode::AHorrorGameGameMode()
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
	static ConstructorHelpers::FObjectFinder<USoundCue>MainMusicAsset(TEXT("/Game/Assets/Sounds/SoundCues/Stage1BGM"));
	if (MainMusicAsset.Succeeded())
	{
		MainCue = MainMusicAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>NervousCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/NervousCue"));
	if (NervousCueAsset.Succeeded())
	{
		NervousCue = NervousCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>ChaseCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/ChaseCue"));
	if (ChaseCueAsset.Succeeded())
	{
		ChaseCue = ChaseCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>MirrorUseSound(TEXT("/Game/Assets/Sounds/SoundCues/MirrorUseCue"));
	if (MirrorUseSound.Succeeded())
	{
		MirrorUseCue = MirrorUseSound.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue>DiedCueAsset(TEXT("/Game/Assets/Sounds/SoundCues/DiedCue"));
	if (DiedCueAsset.Succeeded())
	{
		DiedCue = DiedCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue>ClearSound(TEXT("/Game/Assets/Sounds/SoundCues/ClearCue"));
	if (ClearSound.Succeeded())
	{
		ClearCue = ClearSound.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue>EndingSound(TEXT("/Game/Assets/Sounds/SoundCues/EndingCue"));
	if (EndingSound.Succeeded())
	{
		EndingCue = EndingSound.Object;
	}

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetAutoActivate(false);

	NervousAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NervAudioComp"));
	NervousAudioComponent->SetAutoActivate(false);
}

void AHorrorGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->SetSound(MainCue);
	//AudioComponent->Play();
	AudioComponent->SetAutoActivate(false);

	NervousAudioComponent->SetSound(NervousCue);
	NervousAudioComponent->SetAutoActivate(false);
}

void AHorrorGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHorrorGameGameMode::StartBackGroundMusic()
{
	AudioComponent->Play();
}

void AHorrorGameGameMode::StopBackGroundMusic()
{
	AudioComponent->Stop();
}

void AHorrorGameGameMode::PlayNervousBackGroundMusic()
{
	AudioComponent->FadeOut(2.f, 0.f);
	NervousAudioComponent->FadeIn(2.0f);
}

void AHorrorGameGameMode::StopNervousBackGroundMusic()
{
	NervousAudioComponent->FadeOut(2.f, 0.f);
	
	AudioComponent->FadeIn(2.f);
}

void AHorrorGameGameMode::PlayChaseBackGroundMusic()
{
	AudioComponent->SetSound(ChaseCue);
	NervousAudioComponent->Stop();
	AudioComponent->Play();
}

void AHorrorGameGameMode::StopChaseBackGroundMusic()
{
	AudioComponent->Stop();
	AudioComponent->SetSound(MainCue);
	PlayNervousBackGroundMusic();
}

void AHorrorGameGameMode::PlayMirrorUseBackGroundMusic()
{
	AudioComponent->Stop();
	AudioComponent->SetSound(MirrorUseCue);
	AudioComponent->Play();
}

void AHorrorGameGameMode::StopMirrorUseBackGroundMusic()
{
	AudioComponent->Stop();
	AudioComponent->SetSound(MainCue);
	PlayNervousBackGroundMusic();
}

void AHorrorGameGameMode::PlayDiedBackGroundMusic() // 사망 시 재생할 사운드
{
	AudioComponent->Stop();
	AudioComponent->SetSound(DiedCue);
	AudioComponent->Play();
}

void AHorrorGameGameMode::PlayClearBackGroundMusic() // 클리어 시 재생할 사운드
{
	AudioComponent->Stop();
	AudioComponent->SetSound(ClearCue);
	AudioComponent->Play();
}

void AHorrorGameGameMode::PlayEndingBackGroundMusic() // 엔딩 시 재생할 사운드
{
	AudioComponent->Stop();
	AudioComponent->SetSound(EndingCue);
	AudioComponent->Play();
}