// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/CigarLighter_cpp.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ACigarLighter_cpp::ACigarLighter_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	CigarLight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CigarLight"));
	CigarLight->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CigarLightObj(TEXT("/Game/Assets/Items/CigarLight"));
	if (CigarLightObj.Succeeded())
	{
		CigarLight->SetStaticMesh(CigarLightObj.Object);
	}
	CigarLight->SetRelativeLocation(DefaultPos);
	CigarLight->SetRelativeScale3D(DefaultScale * 0.09f);
	CigarLight->SetCollisionProfileName("ItemObjects");

	/* Deprecated
	FireLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLightFire"));
	FireLight->Mobility = EComponentMobility::Movable;
	FireLight->SetupAttachment(CigarLight);
	FireLight->SetRelativeLocation(FVector(-20.f, 0.f, 0.f));
	FireLight->SetAttenuationRadius(1600.f);
	FireLight->SetVisibility(false);
	FireLight->SetIntensity(5000.f);
	FireLight->SetLightFColor(FColor(255.f, 188.f, 124.f));

	CigarLightOnSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarLightOnSound"));
	CigarLightOnSound->SetupAttachment(CigarLight);
	static ConstructorHelpers::FObjectFinder<USoundCue> IgniteSound(TEXT("/Game/Assets/Sounds/SoundCues/CigarLightCue"));
	if (IgniteSound.Succeeded())
	{
		CigarLightOnSound->SetSound(IgniteSound.Object);
		CigarLightOnSound->OnAudioFinished.AddDynamic(this, &ACigarLighter_cpp::CigarLightOn);
	}

	CigarLightOffSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CigarLightOffSound"));
	CigarLightOffSound->SetupAttachment(CigarLight);
	static ConstructorHelpers::FObjectFinder<USoundCue> TurnOffSound(TEXT("/Game/Assets/Sounds/SoundCues/CigarLightOffCue"));
	if (TurnOffSound.Succeeded())
	{
		CigarLightOffSound->SetSound(TurnOffSound.Object);
	}

	bIsCigarLightOn = false;*/
}

// 플레이어가 라이터와 상호작용할 때 작동할 함수.
void ACigarLighter_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 라이터를 얻는 메서드를 호출함.
	Player->AddCigarLight();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 라이터를 처음 얻은 상태라면 라이터 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item1_CigarLighter)
			{
				SaveData->CollectArchives.Item1_CigarLighter = true;
				Player->SetArchiveGetText(NSLOCTEXT("ACigarLighter_cpp", "Get_Lighter", "Lighter\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void ACigarLighter_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	/* Deprecated
	if (bIsCigarLightOn) {
		FireLight->SetVisibility(false);
		bIsCigarLightOn = false;
		CigarLightOffSound->Play();
	}
	else if (!bIsCigarLightOn)
	{
		CigarLightOnSound->Play();
	}*/
	// Player->CurrentItem();
}

// Deprecated
//void ACigarLighter_cpp::CigarLightOn()
//{
//	FireLight->SetVisibility(true);
//	bIsCigarLightOn = true;
//}