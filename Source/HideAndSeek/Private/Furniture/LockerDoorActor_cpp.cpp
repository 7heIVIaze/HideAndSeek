// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/LockerDoorActor_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Player/HorrorGameCharacter.h"
#include "HUD/GameUI.h"

// Sets default values
ALockerDoorActor_cpp::ALockerDoorActor_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoor"));
	DoorMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_Door(TEXT("/Game/Assets/Furniture/Loker_LockerDoor"));
	if (Locker_Door.Succeeded())
	{
		DoorMesh->SetStaticMesh(Locker_Door.Object);
	}

	LockerLockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lock"));
	LockerLockMesh->SetupAttachment(DoorMesh);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker_LockObj(TEXT("/Game/Assets/Furniture/Lock"));
	if (Locker_LockObj.Succeeded())
	{
		LockerLockMesh->SetStaticMesh(Locker_LockObj.Object);
	}
}

// Called when the game starts or when spawned
void ALockerDoorActor_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 문이 잠겨있지 않는 경우, 자물쇠 메시를 없앰,
	if (!bIsDoorLocked)
	{
		bIsDoorLocked = false;
		LockerLockMesh->DestroyComponent();
	}
	
	// 타임라인 커브 값이 존재하면, 타임라인을 설정하고, 재생할 때 호출할 콜백 함수를 바인딩함
	if (OpenAndCloseCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &ALockerDoorActor_cpp::DoorOpen);
		OpenAndCloseTimeline.AddInterpFloat(OpenAndCloseCurveFloat, TimelineProgress);
	}
}

// Called every frame
void ALockerDoorActor_cpp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OpenAndCloseTimeline.TickTimeline(DeltaTime);
}

// 플레이어가 상호작용할 때 작동할 함수.
void ALockerDoorActor_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	if (!OnInteractionMessage.IsAlreadyBound(Player->GameUIWidget, &UGameUI::ShowMessage))
	{
		OnInteractionMessage.AddDynamic(Player->GameUIWidget, &UGameUI::ShowMessage);
	}

	// 문이 잠긴 경우, 잠겨있다고 플레이어에게 알려줌.
	if (bIsDoorLocked)
	{
		OnInteractionMessage.Broadcast(NSLOCTEXT("LockerMessage", "LockerLocked", "The locker is locked."));
	}
	// 문이 잠기지 않았을 경우
	else
	{
		// 문이 닫힌 경우, 문이 열리는 소리를 재생하고 타임라인을 재생함.
		if (bIsDoorClosed)
		{
			if (DoorOpenSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorOpenSound, GetActorLocation());
			}

			OpenAndCloseTimeline.Play();
		}
		// 문이 열린 경우, 문이 닫히는 소리를 재생하고 타임라인을 역재생함.
		else
		{
			if (DoorCloseSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, DoorCloseSound, GetActorLocation());
			}

			OpenAndCloseTimeline.Reverse();
		}

		// 문이 닫혀있는지에 대한 값을 반전시킴.
		bIsDoorClosed = !bIsDoorClosed;
	}
}

// 문을 여는 효과를 줄 콜백 함수.
void ALockerDoorActor_cpp::DoorOpen(float Value)
{
	// 문을 여닫는 효과를 줌.
	FRotator Rotator = FRotator(0.0f, DoorRotateAngle * Value, 0.0f);
	DoorMesh->SetRelativeRotation(Rotator);
}

// 플레이어가 아이템을 사용할 때 작동할 함수.
bool ALockerDoorActor_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	// 문이 잠겨있는 경우
	if (bIsDoorLocked)
	{
		// 잠금을 해제했다고 알리고, 자물쇠 메시를 제거함.
		Player->bIsFinishUnlock = true;
		bIsDoorLocked = false;
		LockerLockMesh->DestroyComponent();
		return true;
	}

	return false;
}