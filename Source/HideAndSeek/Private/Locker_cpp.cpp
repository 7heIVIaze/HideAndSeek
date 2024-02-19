// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Locker_cpp.h"
#include "Items/Items.h"

// Sets default values
ALocker_cpp::ALocker_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

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

	if (Item->GetChildActorClass() != nullptr)
	{
		bIsItemSpawned = true;
	}
	
	/*float RandomValue = FMath::FRandRange(0.0f, 1.0f);

	int idx = -1;

	if (RandomValue > fNoItemProbability)
	{
		float fTotalProbability = 1.0f - fNoItemProbability;
		RandomValue = FMath::FRandRange(0.0f, fTotalProbability);

		for (int i = 0; i < ItemProbability.Num(); ++i)
		{
			if (i > 0)
			{
				if (RandomValue < ItemProbability[i] && RandomValue >= ItemProbability[i - 1])
				{
					idx = i;
					break;
				}
			}
			else
			{
				if (RandomValue < ItemProbability[i] && RandomValue >= 0)
				{
					idx = i;
					break;
				}
			}
		}

		if (idx >= 0)
		{
			Item->SetChildActorClass(Items[idx]);
		}
	}*/
}

// Called every frame
//void ALocker_cpp::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

bool ALocker_cpp::SetSpawnItem(TSubclassOf<AActor> inItem)
{
	if (!GetIsItemSpawned()) // 아이템이 없는 경우에만
	{
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