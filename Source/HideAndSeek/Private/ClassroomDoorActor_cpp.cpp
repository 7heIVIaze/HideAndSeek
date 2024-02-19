// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com


#include "ClassroomDoorActor_cpp.h"
#include "ClassroomDoors_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"

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

	DoorMesh->SetCollisionProfileName("OpenedDoor");
}

// Called when the game starts or when spawned
void AClassroomDoorActor_cpp::BeginPlay()
{
	Super::BeginPlay();

	SetDoorCollision(false);
	
	ClosedLoc = FVector(0.0f, 0.0f, 0.0f);
	OpenLoc = FVector(0.0f, DoorMovement, 0.0f);
	
	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AClassroomDoorActor_cpp::DoorOpen);
		
		OpenAndClose.AddInterpFloat(CurveFloat, TimelineProgress);


		FOnTimelineEvent OpenAndCloseFinishedCallback;
		OpenAndCloseFinishedCallback.BindDynamic(this, &AClassroomDoorActor_cpp::ChangeDoorCollision);

		OpenAndClose.SetTimelineFinishedFunc(OpenAndCloseFinishedCallback);
	}
}

// Called every frame
void AClassroomDoorActor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndClose.TickTimeline(DeltaTime);
}

void AClassroomDoorActor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsDoorLocked)
	{
		if (DoorSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
		}
		if (bIsDoorClosed)
		{
			OpenAndClose.Play();
		//	DoorMesh->SetCollisionProfileName("OpenedDoor");
		}
		else
		{
			OpenAndClose.Reverse();
		//	DoorMesh->SetCollisionProfileName("ClosedDoor");
		}

		bIsDoorClosed = !bIsDoorClosed;
		CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
	}
	else
	{
		Player->SetErrorText(NSLOCTEXT("AClassroomDoorActor_cpp", "When_Classroom_Door_Locked", "Locked"), 3);
	}
}

void AClassroomDoorActor_cpp::AIInteract(AActor* Creature)
{
	if (!bIsDoorBroken) // 문이 부서진 상태가 아닌 상황이여야 상호작용 가능
	{
		if (bIsDoorLocked) // 만약 문이 잠긴 상황이라면
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Door Is Locked!")));
			if (auto Reaper = Cast<AReaper_cpp>(Creature))
			{
				Reaper->SetIsStop(true);
				InteractingCreatures.Add(Reaper);
			}
			else if (auto Runner = Cast<ARunner_cpp>(Creature))
			{
				Runner->SetIsStop(true);
				InteractingCreatures.Add(Runner);
			}
			else if (auto Brute = Cast<ABrute_cpp>(Creature))
			{
				Brute->SetIsStop(true);
				InteractingCreatures.Add(Brute);
			}
			BreakDoor();
		}
		else
		{
			if (bIsDoorClosed && bIsOtherDoorClosed) // 현재 문과 옆 문도 닫힌 상황이면
			{
				//		USoundCue* DoorSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/DoorOpenSoundCue"));
				if (DoorSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, DoorSound, GetActorLocation());
				}
				OpenAndClose.Play();
				//	Door->SetCollisionProfileName("OpenedDoor");
					// AI가 열린 문을 다시 닫지 않도록 하기 위해 OpenedDoor로 콜리전 변경함. 더불어 열린 문에 끼이지 않도록 하기 위함도 있음
				bIsDoorClosed = false;
				CD_Manager->SetKnowOtherDoorOpen(bIsDoorClosed);
			}
		}
	}
}

void AClassroomDoorActor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	if (bIsDoorLocked)
	{
		bIsDoorLocked = false;
		Lock->DestroyComponent();
		CD_Manager->SetDoorUnlock();
	//	DoorMesh->SetCollisionProfileName("ClosedDoor");
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
	DoorMesh->SetCollisionProfileName("ClosedDoor");
}

void AClassroomDoorActor_cpp::SetKnowOtherDoorOpen(bool value)
{
	bIsOtherDoorClosed = value;
}

void AClassroomDoorActor_cpp::SetDoorCollision(bool inIsPlayerNear)
{
	if (inIsPlayerNear) // true라면 플레이어가 근처에 있는 것이기 때문에 콜리전(물리적 충돌) 활성화
	{
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Block);
	}
	else // false라면 근처에 플레이어가 없는 것이기 때문에 콜리전(물리적 충돌) 비활성화
	{
		// Door->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		DoorMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
		// Door->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore);
	}
}

void AClassroomDoorActor_cpp::ChangeDoorCollision()
{
	if (bIsDoorClosed)
	{
		DoorMesh->SetCollisionProfileName("ClosedDoor");
	}
	else
	{
		DoorMesh->SetCollisionProfileName("OpenedDoor");
	}
}

void AClassroomDoorActor_cpp::BreakDoor()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("BreakDoor Called!")));

	//FActorSpawnParameters spawnParams;
	DoorMesh->SetSimulatePhysics(true);
	//Door->SetCollisionProfileName(TEXT("NoCollision"));
	
	for (AActor* InteractCreature : InteractingCreatures)
	{
		if (AReaper_cpp* Reaper = Cast<AReaper_cpp>(InteractCreature))
		{
			Reaper->SetIsStop(false);
		}
		else if (ARunner_cpp* Runner = Cast<ARunner_cpp>(InteractCreature))
		{
			Runner->SetIsStop(false);
		}
		else if (ABrute_cpp* Brute = Cast<ABrute_cpp>(InteractCreature))
		{
			Brute->SetIsStop(false);
		}
	}
	InteractingCreatures.Empty();
	bIsDoorLocked = false;
	bIsDoorBroken = true;

	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
		DoorMesh->SetSimulatePhysics(false);
		DoorMesh->SetCollisionProfileName(TEXT("NoCollision"));

		GetWorld()->GetTimerManager().ClearTimer(Timer);
	}), 5.f, false);

	CD_Manager->BreakDoor();
}