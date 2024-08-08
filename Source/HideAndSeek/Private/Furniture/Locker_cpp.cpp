// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Furniture/Locker_cpp.h"
#include "Items/Items.h"

// Sets default values
ALocker_cpp::ALocker_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	LockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Locker"));
	LockerMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Locker(TEXT("/Game/Assets/Furniture/Loker_Locker"));
	if (Locker.Succeeded())
	{
		LockerMesh->SetStaticMesh(Locker.Object);
	}
	LockerMesh->SetRelativeLocation(DefaultPos);
	LockerMesh->SetRelativeScale3D(DefaultScale);
	LockerMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("Item"));
	Item->SetupAttachment(LockerMesh);
	Item->SetRelativeLocation(FVector(0.f, 0.f, 20.f));

	bIsItemSpawned = false;
}

// Called when the game starts or when spawned
void ALocker_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 아이템이 이미 있는 경우, 아이템이 스폰되어있다고 설정함.
	if (Item->GetChildActorClass() != nullptr)
	{
		bIsItemSpawned = true;
	}
}

// 아이템을 스폰시키는 함수.
bool ALocker_cpp::SetSpawnItem(TSubclassOf<AActor> inItem)
{
	// 아이템이 없는 경우에만
	if (!GetIsItemSpawned()) 
	{
		// 아이템을 스폰시키고, 아이템이 스폰되었다고 설정함.
		Item->SetChildActorClass(inItem);
		bIsItemSpawned = true;

		return true;
	}

	return false;
}

bool ALocker_cpp::GetIsItemSpawned()
{
	return bIsItemSpawned;
}