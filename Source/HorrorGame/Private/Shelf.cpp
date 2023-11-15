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

	Middle_Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("MiddleItem"));
	Middle_Item->SetupAttachment(ShelfMesh);

	Lower_Item = CreateDefaultSubobject<UChildActorComponent>(TEXT("LowerItem"));
	Lower_Item->SetupAttachment(ShelfMesh);
}

// Called when the game starts or when spawned
void AShelf::BeginPlay()
{
	Super::BeginPlay();
	
	SetUpperItem();
	SetMiddleItem();
	SetLowerItem();
}

void AShelf::SetUpperItem()
{
	// float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("First Random Value: %f"), RandomValue));*/
	int idx = -1;

	if (RandomValue > fNoItemProbability)
	{
		float fTotalProbability = 1.0f - fNoItemProbability;
		RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Second Random Value: %f"), RandomValue));*/

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
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *Items[idx]->GetName()));*/
			Upper_Item->SetChildActorClass(Items[idx]);
		}
	}
}

void AShelf::SetMiddleItem()
{
	// float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("First Random Value: %f"), RandomValue));*/
	int idx = -1;

	if (RandomValue > fNoItemProbability)
	{
		float fTotalProbability = 1.0f - fNoItemProbability;
		RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Second Random Value: %f"), RandomValue));*/

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
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *Items[idx]->GetName()));*/
			Middle_Item->SetChildActorClass(Items[idx]);
		}
	}
}

void AShelf::SetLowerItem()
{
	// float fNoItemProbability = 0.5f; // 아이템이 안 나올 확률
	float RandomValue = FMath::FRandRange(0.0f, 1.0f);
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("First Random Value: %f"), RandomValue));*/
	int idx = -1;

	if (RandomValue > fNoItemProbability)
	{
		float fTotalProbability = 1.0f - fNoItemProbability;
		RandomValue = FMath::FRandRange(0.0f, fTotalProbability);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Second Random Value: %f"), RandomValue));*/

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
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Class: %s"), *Items[idx]->GetName()));*/
			Lower_Item->SetChildActorClass(Items[idx]);
		}
	}
}