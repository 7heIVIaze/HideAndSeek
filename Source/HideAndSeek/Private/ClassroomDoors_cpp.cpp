// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "ClassroomDoors_cpp.h"
#include "ClassroomDoorActor_cpp.h"
#include "Components/BoxComponent.h"
#include "AI/CreatureClass.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AClassroomDoors_cpp::AClassroomDoors_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultSceneRoot;

	LeftDoorActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeftDoor"));
	LeftDoorActor->SetupAttachment(DefaultSceneRoot);

	RightDoorActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("RightDoor"));
	RightDoorActor->SetupAttachment(DefaultSceneRoot);

	PlayerOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerOverlapBox"));
	PlayerOverlapBox->SetupAttachment(DefaultSceneRoot);
	PlayerOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AClassroomDoors_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.AddDynamic(this, &AClassroomDoors_cpp::PlayerBoxEndOverlap);

	/*CreatureOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CreatureOverlapBox"));
	CreatureOverlapBox->SetupAttachment(DefaultSceneRoot);
	CreatureOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AClassroomDoors_cpp::CreatureBoxBeginOverlap);*/

	bIsLocked = true;
}

// Called when the game starts or when spawned
void AClassroomDoors_cpp::BeginPlay()
{
	Super::BeginPlay();

	LeftDoor = Cast<AClassroomDoorActor_cpp>(LeftDoorActor->GetChildActor());
	RightDoor = Cast<AClassroomDoorActor_cpp>(RightDoorActor->GetChildActor());

	LeftDoor->CD_Manager = this;
	RightDoor->CD_Manager = this;

	if (!bIsLocked)
	{
		LeftDoor->SetDoorUnlock();
		RightDoor->SetDoorUnlock();
	}
	SetDoorCollision(false);
}

// Called every frame
void AClassroomDoors_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (!LeftDoor->bIsDoorLocked || !RightDoor->bIsDoorLocked) // if one of them is false
	//{
	//	
	//	if (LeftDoor->bIsDoorLocked)
	//	{
	//		LeftDoor->SetDoorUnlock();
	//	}
	//	if (RightDoor->bIsDoorLocked)
	//	{
	//		RightDoor->SetDoorUnlock();
	//	}
	//}

	//RightDoor->SetKnowOtherDoorOpen(LeftDoor->bIsDoorClosed);
	//LeftDoor->SetKnowOtherDoorOpen(RightDoor->bIsDoorClosed);
}

void AClassroomDoors_cpp::SetDoorCollision(bool value)
{
	bNearPlayer = value;
	LeftDoor->SetDoorCollision(value);
	RightDoor->SetDoorCollision(value);
}

void AClassroomDoors_cpp::SetDoorUnlock()
{
	bIsLocked = false;
	if (LeftDoor->bIsDoorLocked)
	{
		LeftDoor->SetDoorUnlock();
	}
	if (RightDoor->bIsDoorLocked)
	{
		RightDoor->SetDoorUnlock();
	}
}

void AClassroomDoors_cpp::SetKnowOtherDoorOpen(bool value)
{
	if (LeftDoor->bIsDoorClosed == RightDoor->bIsDoorClosed)
	{
		bIsDoorClose = true;
	}
	else
	{
		bIsDoorClose = false;
	}

	// deprecated
	if (LeftDoor->bIsOtherDoorClosed)
	{
		LeftDoor->SetKnowOtherDoorOpen(value);
	}
	if (RightDoor->bIsOtherDoorClosed)
	{
		RightDoor->SetKnowOtherDoorOpen(value);
	}
}


void AClassroomDoors_cpp::PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			SetDoorCollision(true);
		}
	}
}

void AClassroomDoors_cpp::PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherComp != nullptr && OtherActor != this)
	{
		if (OtherActor->IsA<AHorrorGameCharacter>()) // 접촉된 액터가 플레이어여야지만 발동
		{
			SetDoorCollision(false);
		}
	}
}

void AClassroomDoors_cpp::BreakDoor()
{
	if (DoorBreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorBreakSound, GetActorLocation());
	}

	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &AClassroomDoors_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &AClassroomDoors_cpp::PlayerBoxEndOverlap);

	PlayerOverlapBox->DestroyComponent();

	if (LeftDoor->bIsDoorBroken)
	{
		RightDoor->BreakDoor();
	}
	if (RightDoor->bIsDoorBroken)
	{
		LeftDoor->BreakDoor();
	}
}