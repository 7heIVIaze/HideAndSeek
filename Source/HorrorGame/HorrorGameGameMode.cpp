// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorrorGameGameMode.h"
#include "HorrorGameCharacter.h"
#include "Blueprint/UserWidget.h"
#include "GameUI.h"
#include "DiedWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "HorrorGamePlayerController.h"
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

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetAutoActivate(false);
}

void AHorrorGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	//
	if (IsValid(LevelWidgetClass))
	{
		MainWidget = Cast<UGameUI>(CreateWidget(GetWorld(), LevelWidgetClass));
		if (IsValid(MainWidget))
		{
			MainWidget->AddToViewport();
		}
	}

	AudioComponent->SetSound(MainCue);
	//AudioComponent->Play();
	AudioComponent->SetAutoActivate(false);
}

void AHorrorGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	//class AHorrorGameCharacter* PlayerCharacter = nullptr;
	//if (PlayerController)
	//{
	//	PlayerCharacter = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());
	//}
	//if (PlayerCharacter)
	//{
	//	/*if (PlayerCharacter->GetStamina() < 200)
	//	{
	//		if (!bIsStaminaWidgetOn)
	//		{
	//			Stamina_HUD->AddToViewport();
	//			bIsStaminaWidgetOn = true;
	//		}
	//	}
	//	else
	//	{
	//		Stamina_HUD->RemoveFromParent();
	//		bIsStaminaWidgetOn = false;
	//	}*/
	//}
	
}

void AHorrorGameGameMode::StartBackGroundMusic()
{
	AudioComponent->Play();
}