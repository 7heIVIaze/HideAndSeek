// Fill out your copyright notice in the Description page of Project Settings.

#include "ComponentAction/PrologueGameMode.h"
#include "HorrorGame/HorrorGameCharacter.h"
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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetAutoActivate(false);
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
}

void APrologueGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}