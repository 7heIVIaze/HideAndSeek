// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/CigarLighter.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ACigarLighter::ACigarLighter()
{
	SetupLightComponent();

}

void ACigarLighter::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->OnAudioFinished.AddDynamic(this, &ACigarLighter::LightOn);
}

void ACigarLighter::SetupLightComponent()
{
	// Create and setup the light component.
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(RootComponent);

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComponent);
	LightComponent->SetupAttachment(RootComp);

	if (PointLight)
	{
		PointLight->SetVisibility(false);
	}
}