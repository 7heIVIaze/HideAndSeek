// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 던져진 타이머가 울릴 때 스폰될 액터임.

#include "Items/ThrownTimer_cpp.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AThrownTimer_cpp::AThrownTimer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

// 타이머가 울리는 효과를 시작할 함수.
void AThrownTimer_cpp::RingingStart()
{
	// 타이머 소리를 재생하고, 소리가 울리고 있음을 설정함.
	TimerSound->Play();
	bIsSoundPlaying = true;
	//SetPlaySound(true);
}

// 오디오 컴포넌트가 종료되면 호출될 콜백 함수.
void AThrownTimer_cpp::RingingEnd()
{
	// 소리고 멈췄음을 설정하고, 이 액터를 제거함.
	bIsSoundPlaying = false;
	Destroy();
}

// Deprecated
//void AThrownTimer_cpp::SetPlaySound(bool bValue)
//{
//	bIsSoundPlaying = bValue;
//}

bool AThrownTimer_cpp::GetPlaySound()
{
	return bIsSoundPlaying;
}