// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalDoor_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMetalDoor_cpp::AMetalDoor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector(0.f, 0.f, 0.f));
	DefaultSceneRoot->SetWorldScale3D(FVector(1.f, 1.f, 1.f));

	RootComponent = DefaultSceneRoot;

	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>DoorMesh(TEXT("/Game/Assets/Furniture/basementDoor"));
	if (DoorMesh.Succeeded())
	{
		Door->SetStaticMesh(DoorMesh.Object);

	}
	Door->SetCollisionProfileName("LockedDoor");

	DoorRotateAngle = 90.f;
}

// Called when the game starts or when spawned
void AMetalDoor_cpp::BeginPlay()
{
	Super::BeginPlay();
	if (!bIsDoorLocked)
	{
		Door->SetCollisionProfileName("UnLockedDoor");
	}

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AMetalDoor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void AMetalDoor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void AMetalDoor_cpp::OnInteract()
{
	if (!bIsDoorLocked)
	{
		if (bIsDoorClosed)
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/MetalDoorSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Play();
			Door->SetCollisionProfileName("LockedDoor");
		}
		else
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/MetalDoorSound2Cue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
			Door->SetCollisionProfileName("UnLockedDoor");
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
}

void AMetalDoor_cpp::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.f, DoorRotateAngle * Value, 0.f);
	Door->SetRelativeRotation(Rotator);
}

void AMetalDoor_cpp::UseInteract()
{
	if (bIsDoorLocked)
	{
		bIsDoorLocked = false;
		Door->SetCollisionProfileName("UnLockedDoor");
	}
}