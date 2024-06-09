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

	// ClassroomDoorActor들을 관리할 Manager임.
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

	bIsLocked = true;
}

// Called when the game starts or when spawned
void AClassroomDoors_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 차일드액터로부터 원본 액터들을 캐스팅하여 CD Maanger를 자신으로 설정함.
	LeftDoor = Cast<AClassroomDoorActor_cpp>(LeftDoorActor->GetChildActor());
	RightDoor = Cast<AClassroomDoorActor_cpp>(RightDoorActor->GetChildActor());

	LeftDoor->CD_Manager = this;
	RightDoor->CD_Manager = this;

	// 문이 열려있으면 양쪽 문에게 알림.
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

// 양쪽 문의 충돌 여부를 설정하는 함수.
void AClassroomDoors_cpp::SetDoorCollision(bool value)
{
	bNearPlayer = value;
	LeftDoor->SetDoorCollision(value);
	RightDoor->SetDoorCollision(value);
}

// 양쪽 문의 잠금 해제하는 함수.
void AClassroomDoors_cpp::SetDoorUnlock()
{
	// 한 쪽 문이 잠금해제되었으면 반대쪽 문도 잠금해제 함.
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

// 양 쪽 문이 열렸는지 확인하도록 할 함수.
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

// 플레이어가 근처에 있으면 양 쪽 문의 충돌을 활성화할 함수.
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

// 플레이어가 근처에 없으면 양쪽 문의 충돌을 비활성화할 함수.
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

// 문을 부수는 함수.
void AClassroomDoors_cpp::BreakDoor()
{
	// 문이 부숴지는 소리를 재생함.
	if (DoorBreakSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DoorBreakSound, GetActorLocation());
	}

	// 충돌체에 바인딩된 콜백 함수들을 제거하고 충돌체를 없앰.
	PlayerOverlapBox->OnComponentBeginOverlap.RemoveDynamic(this, &AClassroomDoors_cpp::PlayerBoxBeginOverlap);
	PlayerOverlapBox->OnComponentEndOverlap.RemoveDynamic(this, &AClassroomDoors_cpp::PlayerBoxEndOverlap);

	PlayerOverlapBox->DestroyComponent();

	// 반대쪽 문도 부수는 작업을 실행함.
	if (LeftDoor->bIsDoorBroken)
	{
		RightDoor->BreakDoor();
	}
	if (RightDoor->bIsDoorBroken)
	{
		LeftDoor->BreakDoor();
	}
}