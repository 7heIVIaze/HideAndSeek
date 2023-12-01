// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Furniture/DistributionBox.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"

// Sets default values
ADistributionBox::ADistributionBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	BoxMesh->SetupAttachment(RootComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_DBoxDoor(TEXT("/Game/Assets/Furniture/SM_DistributionBox_Body"));
	if (SM_DBoxDoor.Succeeded())
	{
		BoxMesh->SetStaticMesh(SM_DBoxDoor.Object);
	}

	SpawnPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComp);
	bIsPowered = false;
}

// Called when the game starts or when spawned
void ADistributionBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADistributionBox::OnInteract(class AHorrorGameCharacter* Player)
{
	if (!bIsPowered)
	{
		bIsPowered = true;

		for (int i = 0; i < BasementDoor.Num(); ++i)
		{
			FVector NewLocation = BasementDoor[i]->GetActorLocation();
			NewLocation.X -= 300;

			BasementDoor[i]->SetActorLocation(NewLocation);
		}

		if (IsValid(Wall))
		{
			Wall->Destroy();
		}
		FRotator Rotator(0.f, 0.f, 0.f);

		if (IsValid(ReaperClass))
		{
			FActorSpawnParameters SpawnParams;
			AReaper_cpp* Reaper = GetWorld()->SpawnActor<AReaper_cpp>(ReaperClass, SpawnPoint->GetComponentLocation(), Rotator, SpawnParams);
			if (Reaper)
			{
				Reaper->bIsCollectMode = false;
				Reaper->SetCurrentStatus(4);
			}
		}
	}
}