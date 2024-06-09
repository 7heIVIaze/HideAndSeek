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

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

	// 게임 시작 시에 울리는 소리가 재생되지 않도록 비활성화함.
	AudioComp->SetAutoActivate(false);
}

// 플레이어가 상호작용할 때 작동할 함수.
void AAlarm::OnInteract()
{
	// 알람이 안 울릴 때 작동함.
	if (!bIsAlarmRing)
	{
		// 소리를 재생하고, 알람이 울리고 있다고 설정함.
		AudioComp->Play();
		SetAlarmRing();
	}
}

void AAlarm::UseInteract()
{
	
}

// 요괴가 상호작용할 때 작동할 함수.
void AAlarm::AIInteract()
{
	// 알람이 울릴 때 작동함.
	if (bIsAlarmRing)
	{
		// 오디오를 종료하고, 알람이 안 울리고 있다고 설정함.
		AudioComp->Stop();
		// SetAlarmRing();
		bIsAlarmRing = false;
	}
}

void AAlarm::SetAlarmRing()
{
	bIsAlarmRing = !bIsAlarmRing;
}
// Called every frame
//void AAlarm::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

