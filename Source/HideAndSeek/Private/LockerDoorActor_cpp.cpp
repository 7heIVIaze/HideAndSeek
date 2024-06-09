// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

	// 문이 잠겨있지 않는 경우, 자물쇠 메시를 없앰,
	if (!bIsLockerLocked)
	{
		bIsLockerLocked = false;
		LockerLockMesh->DestroyComponent();
	}
	
	// 타임라인 커브 값이 존재하면, 타임라인을 설정하고, 재생할 때 호출할 콜백 함수를 바인딩함
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

// 플레이어가 상호작용할 때 작동할 함수.
void ALockerDoorActor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠기지 않았을 경우
	if (!bIsLockerLocked)
	{
		// 문이 닫힌 경우, 문이 열리는 소리를 재생하고 타임라인을 재생함.
		if (bIsLockerClosed)
		{
			LockerOpenSound->Play();
			OpenAndClose.Play();
		}
		// 문이 열린 경우, 문이 닫히는 소리를 재생하고 타임라인을 역재생함.
		else
		{
			LockerSound->Play();
			OpenAndClose.Reverse();
		}

		// 문이 닫혀있는지에 대한 값을 반전시킴.
		bIsLockerClosed = !bIsLockerClosed;
	}
	// 문이 잠긴 경우, 잠겨있다고 플레이어에게 알려줌.
	else
	{
		Player->SetErrorText(NSLOCTEXT("ALockerDoorActor_cpp", "When_Locker_Locked", "Locked"), 3);
	}
}

// 문을 여는 효과를 줄 콜백 함수.
void ALockerDoorActor_cpp::OpenDoor(float Value)
{
	// 문을 여닫는 효과를 줌.
	FRotator Rotator = FRotator(0.0f, LockerDoorRotateAngle * Value, 0.0f);
	LockerDoorMesh->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
void ALockerDoorActor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠겨있는 경우
	if (bIsLockerLocked)
	{
		// 잠금을 해제했다고 알리고, 자물쇠 메시를 제거함.
		Player->bIsFinishUnlock = true;
		bIsLockerLocked = false;
		LockerLockMesh->DestroyComponent();
	}
}