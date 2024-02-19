// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "LockerDoorActor_cpp.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "HideAndSeek/HorrorGameCharacter.h"

// Sets default values
ALockerDoorActor_cpp::ALockerDoorActor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	LockerDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoor"));
	LockerDoorMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_Door(TEXT("/Game/Assets/Furniture/Loker_LockerDoor"));
	if (Locker_Door.Succeeded())
	{
		LockerDoorMesh->SetStaticMesh(Locker_Door.Object);
	}
	LockerDoorMesh->SetRelativeLocation(FVector(41.f, 45.f, -50.f));
	LockerDoorMesh->SetRelativeScale3D(DefaultScale);

	LockerDoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoorFrame"));
	LockerDoorFrame->SetupAttachment(LockerDoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_Frame(TEXT("/Game/Assets/Furniture/Loker_LockerFrame"));
	if (Locker_Frame.Succeeded())
	{
		LockerDoorFrame->SetStaticMesh(Locker_Frame.Object);
	}
	LockerDoorFrame->SetRelativeLocation(FVector(-41.f, -45.f, 50.f));
	LockerDoorFrame->SetRelativeScale3D(DefaultScale);
	LockerDoorFrame->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	LockerDoorLock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoorLock"));
	LockerDoorLock->SetupAttachment(LockerDoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_Lock(TEXT("/Game/Assets/Furniture/Loker_LockerLock"));
	if (Locker_Lock.Succeeded())
	{
		LockerDoorLock->SetStaticMesh(Locker_Lock.Object);
	}
	LockerDoorLock->SetRelativeLocation(FVector(-41.f, -45.f, 50.f));
	LockerDoorLock->SetRelativeScale3D(DefaultScale);
	LockerDoorLock->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	
	LockerLockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	LockerLockMesh->SetupAttachment(LockerDoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_LockObj(TEXT("/Game/Assets/Furniture/Lock"));
	if (Locker_LockObj.Succeeded())
	{
		LockerLockMesh->SetStaticMesh(Locker_LockObj.Object);
	}
	LockerLockMesh->SetRelativeLocation(FVector(-77.f, 7.f, 49.f));
	LockerLockMesh->SetRelativeScale3D(DefaultScale * 2);
	LockerLockMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	LockerSound = CreateDefaultSubobject<UAudioComponent>(TEXT("CloseSound"));
	LockerSound->SetupAttachment(DefaultSceneRoot);

	LockerOpenSound = CreateDefaultSubobject<UAudioComponent>(TEXT("OpenSound"));
	LockerOpenSound->SetupAttachment(DefaultSceneRoot);
	//LockerSound->SetAutoActivate(false);
	LockerOpenSound->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ALockerDoorActor_cpp::BeginPlay()
{
	Super::BeginPlay();

	if (!bIsLockerLocked)
	{
		bIsLockerLocked = false;
		LockerLockMesh->DestroyComponent();
	}
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALockerDoorActor_cpp::OpenDoor);
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void ALockerDoorActor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void ALockerDoorActor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsLockerLocked)
	{
		if (bIsLockerClosed)
		{
			LockerOpenSound->Play();
			OpenAndClose.Play();
		}
		else
		{
			LockerSound->Play();
			OpenAndClose.Reverse();
		}
		bIsLockerClosed = !bIsLockerClosed;
	}
	else
	{
		Player->SetErrorText(NSLOCTEXT("ALockerDoorActor_cpp", "When_Locker_Locked", "Locked"), 3);
	}
}

void ALockerDoorActor_cpp::OpenDoor(float Value)
{
	FRotator Rotator = FRotator(0.0f, LockerDoorRotateAngle * Value, 0.0f);
	LockerDoorMesh->SetRelativeRotation(Rotator);
}

void ALockerDoorActor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (bIsLockerLocked)
	{
		bIsLockerLocked = false;
		LockerLockMesh->DestroyComponent();
	}
}