// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LightSwitch.h"
#include "SchoolLight.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALightSwitch::ALightSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchFrame"));
	MainMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MainObj(TEXT("/Game/Assets/Furniture/LightSwitch_Main"));
	if (MainObj.Succeeded())
	{
		MainMesh->SetStaticMesh(MainObj.Object);
	}

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	SwitchMesh->SetupAttachment(MainMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SwitchObj(TEXT("/Game/Assets/Furniture/LightSwitch_Switch"));
	if (SwitchObj.Succeeded())
	{
		SwitchMesh->SetStaticMesh(SwitchObj.Object);
	}

	bIsSwitchOn = false;
}

// Called when the game starts or when spawned
void ALightSwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALightSwitch::OnInteract()
{
	//USoundCue* LightSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/LightSwitchCue"));
	if (LightSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LightSound, GetActorLocation());
	}
	SetSwitchStatus();
}

void ALightSwitch::SetSwitchStatus()
{
	bIsSwitchOn = !bIsSwitchOn;
	if (bIsSwitchOn)
	{
		SwitchMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}
	else
	{	
		SwitchMesh->SetRelativeRotation(FRotator(0.f, 10.f, 0.f));
	}
	SetLightsStatus();
}

void ALightSwitch::SetLightsStatus()
{
	for (auto Light : Lights)
	{
		Cast<ASchoolLight>(Light)->SetLightStatus();
	}
}