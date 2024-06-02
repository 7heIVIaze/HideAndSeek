// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Alarm.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AAlarm::AAlarm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	AlarmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Alarm"));
	AlarmMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>AlarmModel(TEXT("/Game/Assets/Furniture/FireAlarm"));
	if (AlarmModel.Succeeded())
	{
		AlarmMesh->SetStaticMesh(AlarmModel.Object);
	}

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComp->SetupAttachment(AlarmMesh);
	LightComp->SetLightFColor(FColor(1.f, 0.f, 0.f, 1.f));

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BellSound"));
	AudioComp->SetupAttachment(AlarmMesh);
	// AudioComp->OnAudioFinished.AddDynamic(this, &AAlarm::SetAlarmRing);

	static ConstructorHelpers::FObjectFinder<USoundCue>AudioCue(TEXT("/Game/Assets/Sounds/SoundCues/AlarmBellSoundCue"));
	if (AudioCue.Succeeded())
	{
		AudioComp->SetSound(AudioCue.Object);
		AudioComp->SetAutoActivate(false);
	}

	bIsAlarmRing = false;
}

// Called when the game starts or when spawned
void AAlarm::BeginPlay()
{
	Super::BeginPlay();

	AudioComp->SetAutoActivate(false);
}

void AAlarm::OnInteract()
{
	if (!bIsAlarmRing)
	{
		AudioComp->Play();
		SetAlarmRing();
	}
}

void AAlarm::UseInteract()
{
	
}

void AAlarm::AIInteract()
{
	if (bIsAlarmRing)
	{
		AudioComp->Stop();
		// SetAlarmRing();
		bIsAlarmRing = false;
	}
}

void AAlarm::SetAlarmRing()
{
	bIsAlarmRing = !bIsAlarmRing;
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("IsAlarmRing: %s"), bIsAlarmRing ? TEXT("true") : TEXT("false")));*/
}
// Called every frame
//void AAlarm::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

