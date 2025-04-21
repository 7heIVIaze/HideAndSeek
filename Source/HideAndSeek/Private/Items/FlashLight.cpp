// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/FlashLight.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFlashLight::AFlashLight()
{
	SetupLightComponent();
}

void AFlashLight::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->OnAudioFinished.AddDynamic(this, &AFlashLight::LightOn);
}

void AFlashLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLightOn)
	{
		Timer += DeltaTime;

		// The battery decreases every 3 seconds.
		if (Timer >= 3.0f)
		{
			// When the flash light is on, the currently selected lighting item is the flash light.n.
			if (auto InventoryComp = GetOwner()->FindComponentByClass<UInventoryComponent>())
			{
				InventoryComp->DecreaseDurability(EItemType::ITEM_Lighting, EItemNumber::ITEM_FlashLight);

			}

			Timer = 0.0f;
		}
	}
}

void AFlashLight::SetupLightComponent()
{
	// Create and setup the light component.
	LightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("LightComponent"));
	LightComponent->SetupAttachment(RootComponent);

	USpotLightComponent* SpotLight = Cast<USpotLightComponent>(LightComponent);
	LightComponent->SetupAttachment(RootComp);

	if (SpotLight)
	{
		SpotLight->SetVisibility(false);
	}
}