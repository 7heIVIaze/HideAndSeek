// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/ThrownTimer_cpp.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AThrownTimer_cpp::AThrownTimer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	TimerMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TimerObj(TEXT("/Game/Assets/Items/timer"));
	if (TimerObj.Succeeded())
	{
		TimerMesh->SetStaticMesh(TimerObj.Object);
	}
	TimerMesh->SetRelativeScale3D(FVector(3.f, 2.f, 3.f));
	TimerMesh->SetCollisionProfileName("NoiseObjects");

	TimerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	TimerSound->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<USoundCue>TimerSoundObj(TEXT("/Game/Assets/Sounds/SoundCues/Timer_Alarm1_Cue"));
	if (TimerSoundObj.Succeeded())
	{
		TimerSound->SetSound(TimerSoundObj.Object);
	}
	TimerSound->SetAutoActivate(false);
	TimerSound->OnAudioFinished.AddDynamic(this, &AThrownTimer_cpp::RingingEnd);
}

// Called when the game starts or when spawned
void AThrownTimer_cpp::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThrownTimer_cpp::RingingStart()
{
	TimerSound->Play();
	SetPlaySound(true);
}

void AThrownTimer_cpp::RingingEnd()
{
	bIsSoundPlaying = false;
	Destroy();
}

void AThrownTimer_cpp::SetPlaySound(bool bValue)
{
	bIsSoundPlaying = bValue;
}

bool AThrownTimer_cpp::GetPlaySound()
{
	return bIsSoundPlaying;
}