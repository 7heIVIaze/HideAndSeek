// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Player/PlayerSpawner.h"
#include "LevelManager.h"

// Sets default values
APlayerSpawner::APlayerSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerSpawner::BindFunction(class ALevelManager* LevelManager)
{
	LevelManager->LevelStartDelegate.AddDynamic(this, &APlayerSpawner::SpawnPlayer);
}

void APlayerSpawner::SpawnPlayer()
{

}