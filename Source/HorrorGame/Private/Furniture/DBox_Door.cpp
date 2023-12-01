// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Furniture/DBox_Door.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "HorrorGame/HorrorGameCharacter.h"

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
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Play();
		}
		else
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorCloseSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
	else
	{
		Player->SetErrorText(TEXT("Locked"), 3);
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
		bIsDoorLocked = false;
	}
}