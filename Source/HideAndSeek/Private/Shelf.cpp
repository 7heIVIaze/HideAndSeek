// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Shelf.h"
#include "Items/Items.h"

// Sets default values
AShelf::AShelf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	ShelfMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shelf"));
	ShelfMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ShelfObj(TEXT("/Game/Assets/Furniture/Wood_Shelf"));
	if (ShelfObj.Succeeded())
	{
		ShelfMesh->SetStaticMesh(ShelfObj.Object);
	}
	ShelfMesh->SetRelativeScale3D(FVector(1.2f));

	Upper_Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("UpperItem"));
	Upper_Item->SetupAttachment(ShelfMesh);

	Upper_Deco_Left = CreateDefaultSubobject<UChildActorComponent>(TEXT("Upper_Deco_Left"));
	Upper_Deco_Left->SetupAttachment(ShelfMesh);

	Upper_Deco_Right = CreateDefaultSubobject<UChildActorComponent>(TEXT("Upper_Deco_Right"));
	Upper_Deco_Right->SetupAttachment(ShelfMesh);

	Middle_Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("MiddleItem"));
	Middle_Item->SetupAttachment(ShelfMesh);

	Middle_Deco_Left = CreateDefaultSubobject<UChildActorComponent>(TEXT("Middle_Deco_Left"));
	Middle_Deco_Left->SetupAttachment(ShelfMesh);

	Middle_Deco_Right = CreateDefaultSubobject<UChildActorComponent>(TEXT("Middle_Deco_Right"));
	Middle_Deco_Right->SetupAttachment(ShelfMesh);

	Lower_Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("LowerItem"));
	Lower_Item->SetupAttachment(ShelfMesh);

	Lower_Deco_Left = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lower_Deco_Left"));
	Lower_Deco_Left->SetupAttachment(ShelfMesh);

	Lower_Deco_Right = CreateDefaultSubobject<UChildActorComponent>(TEXT("Lower_Deco_Right"));
	Lower_Deco_Right->SetupAttachment(ShelfMesh);

	bIsUpperItemSpawned = false;
	bIsMiddleItemSpawned = false;
	bIsLowerItemSpawned = false;
}

// Called when the game starts or when spawned
void AShelf::BeginPlay()
{
	Super::BeginPlay();
	
	if (Upper_Item->GetChildActorClass() != nullptr)
	{
		bIsUpperItemSpawned = true;
	}

	if (Middle_Item->GetChildActorClass() != nullptr)
	{
		bIsMiddleItemSpawned = true;
	}

	if (Lower_Item->GetChildActorClass() != nullptr)
	{
		bIsLowerItemSpawned = true;
	}

	if (Decorators.Num() > 0)
	{
		float RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Upper_Deco_Left->SetChildActorClass(Decorators[randIdx]);
		}

		RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Upper_Deco_Right->SetChildActorClass(Decorators[randIdx]);
		}

		RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Middle_Deco_Left->SetChildActorClass(Decorators[randIdx]);
		}

		RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Middle_Deco_Right->SetChildActorClass(Decorators[randIdx]);
		}

		RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Lower_Deco_Left->SetChildActorClass(Decorators[randIdx]);
		}

		RandomProbability = FMath::RandRange(0.f, 1.f);
		if (RandomProbability > 0.3f) // 30퍼센트보다 더 큰 확률이면(즉 70퍼센트로 데코 생성)
		{
			int32 randIdx = FMath::RandRange(0, Decorators.Num() - 1);
			Lower_Deco_Right->SetChildActorClass(Decorators[randIdx]);
		}
	}

	/*SetUpperItem();
	SetMiddleItem();
	SetLowerItem();*/
}

void AShelf::SetUpperItem()
{
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
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
			Upper_Item->SetChildActorClass(Items[idx]);
		}
	}
}

void AShelf::SetMiddleItem()
{
	
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	
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
			Middle_Item->SetChildActorClass(Items[idx]);
		}
	}
}

void AShelf::SetLowerItem()
{
	
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	
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
			Lower_Item->SetChildActorClass(Items[idx]);
		}
	}
}

bool AShelf::SetSpawnItem(TSubclassOf<AActor> inItem)
{
	int32 RandPosition = FMath::RandRange(0, 2); // 0: Lower, 1: Middle, 2: Upper

	if (RandPosition == 0)
	{
		if (!GetIsLowerItemSpawned())
		{
			Lower_Item->SetChildActorClass(inItem);
			return true;
		}
	}
	else if (RandPosition == 1)
	{
		if (!GetIsMiddleItemSpawned())
		{
			Middle_Item->SetChildActorClass(inItem);
			return true;
		}
	}
	else if (RandPosition == 2)
	{
		if (!GetIsUpperItemSpawned())
		{
			Upper_Item->SetChildActorClass(inItem);
			return true;
		}
	}

	return false;
}

bool AShelf::GetIsUpperItemSpawned()
{
	return bIsUpperItemSpawned;
}

bool AShelf::GetIsMiddleItemSpawned()
{
	return bIsMiddleItemSpawned;
}

bool AShelf::GetIsLowerItemSpawned()
{
	return bIsLowerItemSpawned;
}