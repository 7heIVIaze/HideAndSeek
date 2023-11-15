// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/CigarLighter_cpp.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
// #include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"

// Sets default values
ACigarLighter_cpp::ACigarLighter_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

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

	/*FireLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("CigarLightFire"));
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

void ACigarLighter_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddCigarLight();
	if (Player->bCanItemGet)
	{
		Destroy();
	}
}

void ACigarLighter_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	/*if (bIsCigarLightOn) {
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
//
//void ACigarLighter_cpp::CigarLightOn()
//{
//	FireLight->SetVisibility(true);
//	bIsCigarLightOn = true;
//}