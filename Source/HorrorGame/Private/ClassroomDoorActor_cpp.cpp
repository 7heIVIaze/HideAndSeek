// Fill out your copyright notice in the Description page of Project Settings.


#include "ClassroomDoorActor_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AClassroomDoorActor_cpp::AClassroomDoorActor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FVector DefaultPos = FVector(0.0f, 0.0f, 0.0f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClassDoor"));
	DoorMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CRDoorObj(TEXT("/Game/Assets/Furniture/newClassroomDoor"));
	if (CRDoorObj.Succeeded())
	{
		DoorMesh->SetStaticMesh(CRDoorObj.Object);
	}
	DoorMesh->SetRelativeLocation(DefaultPos);
	DoorMesh->SetRelativeScale3D(DefaultScale);

	Lock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	Lock->SetupAttachment(DoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>LockMesh(TEXT("/Game/Assets/Furniture/Lock"));
	if (LockMesh.Succeeded())
	{
		Lock->SetStaticMesh(LockMesh.Object);
		Lock->SetRelativeLocation(FVector(168.0f, 5.0f, 170.0f));
		Lock->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	}

	DoorMesh->SetCollisionProfileName("LockedClass");
}

// Called when the game starts or when spawned
void AClassroomDoorActor_cpp::BeginPlay()
{
	Super::BeginPlay();

	DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	if (!bIsDoorLocked)
	{
		Lock->DestroyComponent();
		DoorMesh->SetCollisionProfileName("UnLockedClass");
	}

	ClosedLoc = FVector(0.0f, 0.0f, 0.0f);
	OpenLoc = FVector(0.0f, DoorMovement, 0.0f);
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AClassroomDoorActor_cpp::DoorOpen);
		
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);
	}
}

// Called every frame
void AClassroomDoorActor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void AClassroomDoorActor_cpp::OnInteract()
{
	if (!bIsDoorLocked)
	{
		USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ClassroomDoorSoundCue"));
		if (DoorSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
		}
		if (bIsDoorClosed)
		{
			OpenAndClose.Play();
			DoorMesh->SetCollisionProfileName("LockedClass");
		}
		else
		{
			OpenAndClose.Reverse();
			DoorMesh->SetCollisionProfileName("UnLockedClass");
		}

		bIsDoorClosed = !bIsDoorClosed;
	}
}

void AClassroomDoorActor_cpp::AIInteract()
{
	if (!bIsDoorLocked)
	{
		bIsDoorClosed = false;
		if (!bIsDoorClosed && bIsOtherDoorClosed)
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ClassroomDoorSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Play();
			DoorMesh->SetCollisionProfileName("LockedClass");
		}
		else if (bIsDoorClosed && !bIsOtherDoorClosed)
		{
			USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ClassroomDoorSoundCue"));
			if (DoorSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
			}
			OpenAndClose.Reverse();
			DoorMesh->SetCollisionProfileName("LockedClass");
		}
	}
}

void AClassroomDoorActor_cpp::UseInteract()
{
	if (bIsDoorLocked)
	{
		bIsDoorLocked = false;
		Lock->DestroyComponent();
		DoorMesh->SetCollisionProfileName("UnLockedClass");
	}
}

void AClassroomDoorActor_cpp::DoorOpen(float Value)
{
	FVector Movement = FVector(DoorMovement * Value, 0.0f, 0.0f);
	DoorMesh->SetRelativeLocation(Movement);
}

void AClassroomDoorActor_cpp::SetDoorUnlock()
{
	bIsDoorLocked = false;
	Lock->DestroyComponent();
	DoorMesh->SetCollisionProfileName("UnLockedClass");
}

void AClassroomDoorActor_cpp::SetKnowOtherDoorOpen(bool value)
{
	bIsOtherDoorClosed = value;
}

void AClassroomDoorActor_cpp::SetDoorCollision(bool value)
{
	if (value) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		DoorMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}

//void AClassroomDoorActor_cpp::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
//	{
//		
//	}
//}

//void AClassroomDoorActor_cpp::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}