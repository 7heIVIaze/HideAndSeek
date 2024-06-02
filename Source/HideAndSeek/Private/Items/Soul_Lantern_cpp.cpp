// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Soul_Lantern_cpp.h"
#include "Sound/SoundCue.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "EngineUtils.h"
#include "Altar_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"

ASoul_Lantern_cpp::ASoul_Lantern_cpp()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern"));
	LanternMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LanternObj(TEXT("/Game/Assets/Items/Lantern"));
	if (LanternObj.Succeeded())
	{
		LanternMesh->SetStaticMesh(LanternObj.Object);
	}
	LanternMesh->SetCollisionProfileName("ItemObjects");

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("SoulLight"));
	Light->SetupAttachment(LanternMesh);

	LightNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Soul"));
	LightNiagara->SetupAttachment(LanternMesh);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>Soul_NS(TEXT("/Game/Assets/Material/NS_Soul"));
	if (Soul_NS.Succeeded())
	{
		LightNiagara->SetAsset(Soul_NS.Object);
	}

	bIsLightOn = false;
	AltarLocation = FVector(0, 0, 0);
}

void ASoul_Lantern_cpp::BeginPlay()
{
	Super::BeginPlay();

	Light->SetVisibility(bIsLightOn);
	LightNiagara->Deactivate();

	for (TActorIterator<AAltar_cpp>entity(GetWorld()); entity; ++entity)
	{
		AAltar_cpp* Altar = *entity;
		if (Altar != nullptr) {
			AltarLocation = Altar->GetActorLocation();
			AltarLocation.Z = 0.f; // 정확한 Z축을 구하기 위한 Cross Product 계산을 위해 0으로 설정
		}
	}
}

void ASoul_Lantern_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLightOn)
	{
		FVector ToTarget = AltarLocation - LightNiagara->GetComponentLocation();
		FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
		LightNiagara->SetWorldRotation(LookAtRotation);
	}
}

void ASoul_Lantern_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	Player->AddLantern();
	if (Player->bCanItemGet)
	{
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item10_SoulLantern) // 영혼 랜턴을 처음 얻은 상태라면
			{
				SaveData->CollectArchives.Item10_SoulLantern = true;
				Player->SetArchiveGetText(NSLOCTEXT("ASoul_Lantern_cpp", "Get_SoulLantern", "Soul Lantern\nis added in archive"));
				SaveData->SaveData();
			}
		}
		Destroy();
	}
}

void ASoul_Lantern_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	
	bIsLightOn = !bIsLightOn;
	Player->bLanternOn = bIsLightOn;
	UE_LOG(LogTemp, Warning, TEXT("Player: LanternOn : %s"), bIsLightOn ? TEXT("true") : TEXT("false"));
	if (bIsLightOn)
	{
		//USoundCue* LanternCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LanternSoundCue"));
		if (LanternCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LanternCue, GetActorLocation());
		}
		LightNiagara->Activate(true);
	}
	else
	{
		LightNiagara->Activate(false);
		LightNiagara->Deactivate();
	}
	Light->SetVisibility(bIsLightOn);

}


void ASoul_Lantern_cpp::HideInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	
	bIsLightOn = !bIsLightOn;
	UE_LOG(LogTemp, Warning, TEXT("Player: LanternOn : %s"), bIsLightOn ? TEXT("true") : TEXT("false"));
	if (bIsLightOn)
	{
		//USoundCue* LanternCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LanternSoundCue"));
		/*if (LanternCue)
		{
			UGameplayStatics::PlaySoundAtLocation(this, LanternCue, GetActorLocation());
		}*/
		LightNiagara->Activate(true);
	}
	else
	{
		LightNiagara->Activate(false);
		LightNiagara->Deactivate();
	}
	Light->SetVisibility(bIsLightOn);

}