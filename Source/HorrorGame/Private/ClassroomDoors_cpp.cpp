// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ClassroomDoors_cpp.h"
#include "ClassroomDoorActor_cpp.h"

// Sets default values
AClassroomDoors_cpp::AClassroomDoors_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FVector DefaultLoc = FVector(0.0f, 0.0f, 0.0f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultLoc);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	//LeftDoor = CreateDefaultSubobject<AClassroomDoorActor_cpp>(TEXT("LeftDoor"));
	//RightDoor = CreateDefaultSubobject<AClassroomDoorActor_cpp>(TEXT("RightDoor"));
	//static ConstructorHelpers::FObjectFinder<AClassroomDoorActor_cpp>DoorActor(TEXT("/Game/Assets/BluePrints/BP_ClassroomDoorActor_cpp"));
	//if (DoorActor.Succeeded())
	//{
	//	LeftDoor = DoorActor.Object;
	//	RightDoor = DoorActor.Object;
	//}
	//LeftDoor->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepWorldTransform);
	//LeftDoor->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	//LeftDoor->SetActorRelativeLocation(FVector(0.5f, 0.0f, 0.0f));
	//RightDoor->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepWorldTransform);

	/*LockMeshFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockFront"));
	LockMeshFront->SetupAttachment(DefaultSceneRoot);
	LockMeshFront->SetRelativeLocation(FVector(5.0f, 10.0f, 180.0f));
	LockMeshFront->SetRelativeScale3D(DefaultScale * 2.0f);

	LockMeshBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockBack"));
	LockMeshBack->SetupAttachment(DefaultSceneRoot);
	LockMeshBack->SetRelativeLocation(FVector(-5.0f, 0.0f, 180.0f));
	LockMeshBack->SetRelativeScale3D(DefaultScale * 2.0f);*/

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh>LockObj(TEXT("/Game/Assets/Furniture/Lock"));
	if (LockObj.Succeeded())
	{
		LockMeshFront->SetStaticMesh(LockObj.Object);
		LockMeshBack->SetStaticMesh(LockObj.Object);
	}*/

	//LeftDoor = CreateDefaultSubobject<AClassroomDoorActor_cpp>(TEXT("LeftDoor"));
	//LeftDoor->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepWorldTransform);
	//LeftDoor->SetActorRelativeLocation(DefaultLoc);
	//LeftDoor->SetActorRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

	//RightDoor = CreateDefaultSubobject<AClassroomDoorActor_cpp>(TEXT("RightDoor"));
	//RightDoor->AttachToComponent(DefaultSceneRoot, FAttachmentTransformRules::KeepWorldTransform);
	//RightDoor->SetActorRelativeLocation(DefaultLoc);
	
}

// Called when the game starts or when spawned
void AClassroomDoors_cpp::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetTimerManager().SetTimer(_BooleanCheckTimer, this, &AClassroomDoors_cpp::ChangeValue, 0.5f, false);
	//if (!LeftDoor->bIsDoorLocked || !RightDoor->bIsDoorLocked) // if one of them is false
	//{
	//	bIsDoorLocked = false;
	//}
	//LeftDoor->bIsDoorLocked = bIsDoorLocked;
	//RightDoor->bIsDoorLocked = bIsDoorLocked;
}

// Called every frame
void AClassroomDoors_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!LeftDoor->bIsDoorLocked || !RightDoor->bIsDoorLocked) // if one of them is false
	{
		// bIsDoorLocked = false;
		/*LockMeshFront->DestroyComponent();
		LockMeshBack->DestroyComponent();*/
		if (LeftDoor->bIsDoorLocked)
			LeftDoor->SetDoorUnlock();
		if (RightDoor->bIsDoorLocked)
			RightDoor->SetDoorUnlock();
		//GetWorld()->GetTimerManager().ClearTimer(_BooleanCheckTimer);
		//return;
	}

	RightDoor->SetKnowOtherDoorOpen(LeftDoor->bIsDoorClosed);
	LeftDoor->SetKnowOtherDoorOpen(RightDoor->bIsDoorClosed);
}

void AClassroomDoors_cpp::SetDoorCollision(bool value)
{
	LeftDoor->SetDoorCollision(value);
	RightDoor->SetDoorCollision(value);
}

//void AClassroomDoors_cpp::ChangeValue()
//{
//	if (!LeftDoor->bIsDoorLocked || !RightDoor->bIsDoorLocked) // if one of them is false
//	{
//		// bIsDoorLocked = false;
//		/*LockMeshFront->DestroyComponent();
//		LockMeshBack->DestroyComponent();*/
//		if(LeftDoor->bIsDoorLocked)
//			LeftDoor->SetDoorUnlock();
//		if(RightDoor->bIsDoorLocked)
//			RightDoor->SetDoorUnlock();
//		//GetWorld()->GetTimerManager().ClearTimer(_BooleanCheckTimer);
//		//return;
//	}
//
//	RightDoor->SetKnowOtherDoorOpen(LeftDoor->bIsDoorClosed);
//	LeftDoor->SetKnowOtherDoorOpen(RightDoor->bIsDoorClosed);
//	GetWorld()->GetTimerManager().ClearTimer(_BooleanCheckTimer);
//	GetWorld()->GetTimerManager().SetTimer(_BooleanCheckTimer, this, &AClassroomDoors_cpp::ChangeValue, 0.5f, false);
//}