// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "CreatureSpawner.h"
#include "EngineUtils.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "PatrolPoint_cpp.h"
#include "HorrorGamePlayerController.h"
#include "HorrorGameGameInstance.h"

// Sets default values
ACreatureSpawner::ACreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
}

// Called when the game starts or when spawned
void ACreatureSpawner::BeginPlay()
{
	Super::BeginPlay();
	world = GetWorld();
	spawnLocation = GetActorLocation();
	rotator = FRotator(0.f, 0.f, 0.f);
	//path = TEXT("/Game/Assets/AI/BP_Reaper");
	//ObjectToSpawn = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	
	// 생성할 적 개체가 있는지 판별함.
	for (int i = 0; i < ObjectToSpawn.Num(); ++i)
	{
		if (ObjectToSpawn[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("ObjectToSpawn Is NULL Pointer"));
		}

		if (IsValid(ObjectToSpawn[i]))
		{
			UE_LOG(LogTemp, Warning, TEXT("ObjectToSpawn is Valid"));
		}
	}
}

// 레벨 매니저가 맵 생성이 다 
void ACreatureSpawner::CreatureSpawn()
{
	FActorSpawnParameters SpawnParams;
	
	//for (int i = 0; i < 2; ++i)
	//{
		// AReaper_cpp* reaper = world->SpawnActor<AReaper_cpp>(ObjectToSpawn[0], spawnLocation, rotator, spawnParams);
	//}

	// 러너와 브루트를 하나씩 스폰함.
	ARunner_cpp* runner = world->SpawnActor<ARunner_cpp>(ObjectToSpawn[0], spawnLocation, rotator, SpawnParams);
	ABrute_cpp* brute = world->SpawnActor<ABrute_cpp>(ObjectToSpawn[1], spawnLocation, rotator, SpawnParams);
}