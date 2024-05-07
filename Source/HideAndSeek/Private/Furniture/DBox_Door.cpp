// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Furniture/DBox_Door.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HideAndSeek/HorrorGameCharacter.h"

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

void ADBox_Door::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsDoorLocked)
	{
		if (bIsDoorClosed)
		{
			OpenSound->Play();
			OpenAndClose.Play();
		}
		else
		{
			CloseSound->Play();
			OpenAndClose.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	else
	{
		Player->SetErrorText(NSLOCTEXT("ADBox_Door", "When_Box_Door_Locked", "Locked"), 3);
	}
}

void ADBox_Door::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	BoxMesh->SetRelativeRotation(Rotator);
}

void ADBox_Door::UseInteract(class AHorrorGameCharacter* Player)
{
	if (bIsDoorLocked)
	{
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
	}
}