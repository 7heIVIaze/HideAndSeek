// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/LightItem.h"
#include "Components/LightComponent.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/Altar_cpp.h"


// Sets default values
ALightItem::ALightItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	LightComponent = nullptr;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->SetupAttachment(RootComp);
	bIsLightOn = false;
}

// Called when the game starts or when spawned
void ALightItem::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->SetActive(false);
}

// Called every frame
void ALightItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALightItem::TurnOn()
{
	if (AudioComponent)
	{
		AudioComponent->Play();
	}

	return true;
}

void ALightItem::LightOn()
{
	bool result = false, bWasLightOn = bIsLightOn;
	if (LightComponent)
	{
		LightComponent->SetVisibility(true);
		bIsLightOn = true;
		Timer = 0.0f;
	}
}

bool ALightItem::TurnOff()
{
	bool result = false, bWasLightOn = bIsLightOn;
	if (LightComponent)
	{
		LightComponent->SetVisibility(false);
		LightComponent->SetIntensity(LightIntensity);
		
		if (TurnoffSoundCue)
		{
			UGameplayStatics::PlaySound2D(this, TurnoffSoundCue);
		}

		bIsLightOn = false;
		Timer = 0.0f;
		return (bWasLightOn != bIsLightOn) ? true : false;
	}

	return false;
}

void ALightItem::SetupDurability(int inDurability)
{
	Durability = inDurability;
}

void ALightItem::SetItemVisibility(bool NewVisibility)
{
	LightComponent->SetHiddenInGame(NewVisibility);
}