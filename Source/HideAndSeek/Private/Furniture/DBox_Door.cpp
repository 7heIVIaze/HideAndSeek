// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DBox_Door.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
ADBox_Door::ADBox_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	BoxMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_DBoxDoor(TEXT("/Game/Assets/Furniture/SM_DistributionBox_Door"));
	if (SM_DBoxDoor.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_DBoxDoor.Object);
	}
	DoorRotateAngle = -90.0f;

	OpenSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DoorOpenSound"));

	CloseSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DoorCloseSound"));
}

// Called when the game starts or when spawned
void ADBox_Door::BeginPlay()
{
	Super::BeginPlay();

	// 문이 열리는 애니메이션을 위해 타임라인을 추가해주고 해당 타임라인에 OpenDoor 함수를 바인딩해줌.
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADBox_Door::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADBox_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용을 할 때 수행할 함수.
void ADBox_Door::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠기지 않은 경우에
	if (!bIsDoorLocked)
	{
		// 문이 닫혀있으면 열고
		if (bIsDoorClosed)
		{
			OpenSound->Play();
			OpenAndClose.Play();
		}
		// 문이 열려있으면 닫음.
		else
		{
			CloseSound->Play();
			OpenAndClose.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	// 문이 잠겨 있는 경우에는 잠겼다는 텍스트를 출력해줌.
	else
	{
		Player->SetErrorText(NSLOCTEXT("ADBox_Door", "When_Box_Door_Locked", "Locked"), 3);
	}
}

// 문을 여닫는 효과를 줄 함수
void ADBox_Door::OpenDoor(float Value)
{
	// 메시가 Yaw 축을 기준으로 최대 90도까지 회전할 수 있도록 설정함
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	BoxMesh->SetRelativeRotation(Rotator);
}

// 문 앞에서 아이템을 사용할 때 작동할 함수.
void ADBox_Door::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠긴 상태에서만 작동
	if (bIsDoorLocked)
	{
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
	}
}