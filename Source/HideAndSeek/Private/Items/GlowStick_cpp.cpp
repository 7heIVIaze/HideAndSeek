// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/GlowStick_cpp.h"
#include "Components/PointLightComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

AGlowStick_cpp::AGlowStick_cpp()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComp);

	/*LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComp->SetupAttachment(MeshComp);*/

	//bIsLightOn = false;
//	LightComp->SetVisibility(false);
}

void AGlowStick_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	Player->AddGlowStick();
	if (Player->bCanItemGet)
	{
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item11_GlowStick)
			{
				SaveData->CollectArchives.Item11_GlowStick = true;
				Player->SetArchiveGetText(NSLOCTEXT("AGlowStick_cpp", "Get_GlowStick", "Glow Stick\nis added in archive"));
				SaveData->SaveData();
			}
		}
		Destroy();
	}
}

void AGlowStick_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, GetActorLocation());
	}
	//bIsLightOn = true;
//	LightComp->SetVisibility(true);
	UMaterialInterface* Material = MeshComp->GetMaterial(1);
	MaterialInstance = MeshComp->CreateDynamicMaterialInstance(1, Material);

	MaterialInstance->SetScalarParameterValue(TEXT("Glow"), 1.0f);
}