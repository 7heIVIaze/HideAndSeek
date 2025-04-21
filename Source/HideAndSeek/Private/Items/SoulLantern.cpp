// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/SoulLantern.h"
#include "Sound/SoundCue.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "EngineUtils.h"
#include "Furniture/Altar_cpp.h"
#include "Player/HorrorGameCharacter.h"

ASoulLantern::ASoulLantern()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupLightComponent();
}

void ASoulLantern::BeginPlay()
{
	Super::BeginPlay();

	UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());

	if (GameInstance)
	{
		HPLossRate = HPLoss * GameInstance->GetHPLossRate();
	}
}

void ASoulLantern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the lantern is on
	if (bIsLightOn)
	{
		Timer += DeltaTime;

		if (Timer >= 1.0f)
		{
			if (auto StatComp = GetOwner()->FindComponentByClass<UPlayerStatComponent>())
			{
				StatComp->TakeDamage(HPLossRate);
			}

			Timer = 0.0f;
		}
	}
}

void ASoulLantern::SetupLightComponent()
{
	// Create and setup the light component.
	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComponent"));

	UPointLightComponent* PointLight = Cast<UPointLightComponent>(LightComponent);
	LightComponent->SetupAttachment(RootComp);

	if (PointLight)
	{
		PointLight->SetVisibility(false);
	}
}