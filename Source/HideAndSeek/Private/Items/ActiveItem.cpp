// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/ActiveItem.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActiveItem::AActiveItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Meshes default settings.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	MeshComponent->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void AActiveItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AActiveItem::UseItem()
{
	return true;
}

void AActiveItem::SetupDurability(int inDurability)
{
	Durability = inDurability;
}

void AActiveItem::SetItemVisibility(bool NewVisibility)
{
	MeshComponent->SetHiddenInGame(NewVisibility);
}