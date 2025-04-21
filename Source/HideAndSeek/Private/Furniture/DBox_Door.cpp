// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/DBox_Door.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"

// Sets default values
ADBox_Door::ADBox_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_DBoxDoor(TEXT("/Game/Assets/Furniture/SM_DistributionBox_Door"));
	if (SM_DBoxDoor.Succeeded())
	{
		DoorMesh->SetStaticMesh(SM_DBoxDoor.Object);
	}

	DoorRotateAngle = -90.0f;
}

// Called when the game starts or when spawned
void ADBox_Door::BeginPlay()
{
	Super::BeginPlay();

	// 문이 열리는 애니메이션을 위해 타임라인을 추가해주고 해당 타임라인에 OpenDoor 함수를 바인딩해줌.
	if (OpenAndCloseCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ADBox_Door::DoorOpen);
		OpenAndCloseTimeline.AddInterpFloat(OpenAndCloseCurveFloat, TimelineProgress);
	}
}

// Called every frame
void ADBox_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndCloseTimeline.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용을 할 때 수행할 함수.
void ADBox_Door::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	// 문이 잠겨 있는 경우에는 잠겼다는 텍스트를 출력해줌.
	if (bIsDoorLocked)
	{
		OnInteractionMessage.Broadcast(NSLOCTEXT("DoorMessage", "DoorLocked", "The door is locked."));
	}
	// 문이 잠기지 않은 경우에
	else
	{
		// 문이 닫혀있으면 열고
		if (bIsDoorClosed)
		{
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}

			OpenAndCloseTimeline.Play();
		}
		// 문이 열려있으면 닫음.
		else
		{
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}

			OpenAndCloseTimeline.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
}

void ADBox_Door::DoorOpen(float inOpenAndCloseCurveFloat)
{
	// 메시가 Yaw 축을 기준으로 최대 90도까지 회전할 수 있도록 설정함
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * inOpenAndCloseCurveFloat, 0.0f);
	DoorMesh->SetRelativeRotation(Rotator);
}

// 문 앞에서 아이템을 사용할 때 작동할 함수.
bool ADBox_Door::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	// 문이 잠긴 상태에서만 작동
	if (bIsDoorLocked)
	{
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
		return true;
	}

	return false;
}