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

//void ACreatureSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	FActorSpawnParameters spawnParams;
//	fPlayTime += DeltaTime;
//	if (count++ > 60)
//	{
//		//for (TActorIterator<APatrolPoint_cpp>entity(world); entity; ++entity)
//		//{
//		//	FString entityName = entity->GetActorLabel(); //->GetName();
//		//	UE_LOG(LogTemp, Warning, TEXT("%s"), *entityName);
//		//	entityName.RemoveFromStart(TEXT("BP_PatrolPoint"));
//		//	int32 idx = FCString::Atoi(*entityName);
//		//	UE_LOG(LogTemp, Warning, TEXT("%d"), idx);
//		//	if (idx > 0 || idx <= 25)
//		//		if (!IsAllLoaded[idx - 1])
//		//			IsAllLoaded[idx - 1] = true;
//		//}
//
//		//if (Check())
//		//{
//		//	FString strText = FString::Printf(TEXT("%2d:%2d"), (int32)fPlayTime / 60, (int32)fPlayTime % 60);
//		//	if (GEngine)
//		//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, strText);
//		//	if (reaper_cnt++ > 3)
//		//		Destroy();
//		//	//AReaper_cpp* reaper = world->SpawnActor<AReaper_cpp>(ObjectToSpawn->GeneratedClass, spawnLocation, rotator, spawnParams);
//		//	AReaper_cpp* reaper = world->SpawnActor<AReaper_cpp>(ObjectToSpawn, spawnLocation, rotator, spawnParams);
//		//	/*if (reaper)
//		//	{
//		//		ULevel* CurrentLevel = world->GetCurrentLevel();
//		//		if (CurrentLevel)
//		//		{
//		//			CurrentLevel->Actors.Add(reaper);
//		//			reaper_cnt++;
//		//			return;
//		//		}
//		//	}*/
//		//	reaper_cnt++;
//		//}
//		count = 0;
//	}
//
//	
//}

//bool ACreatureSpawner::Check()
//{
//	for (int i = 0; i < 25; ++i)
//	{
//		if (!IsAllLoaded[i]) return false;
//		UE_LOG(LogTemp, Warning, TEXT("IsAllLoaded[%d] is true"), i);
//	}
//	
//	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(GetWorld()->GetFirstPlayerController());
//	/*UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
//	GameInstance->EndLoadingScreen(GetWorld());*/
//	PlayerController->ShowMainUI();
//	return true;
//}

void ACreatureSpawner::CreatureSpawn()
{
	FActorSpawnParameters SpawnParams;
	//for (int i = 0; i < 2; ++i)
	//{
		// AReaper_cpp* reaper = world->SpawnActor<AReaper_cpp>(ObjectToSpawn[0], spawnLocation, rotator, spawnParams);
	ARunner_cpp* runner = world->SpawnActor<ARunner_cpp>(ObjectToSpawn[0], spawnLocation, rotator, SpawnParams);
	ABrute_cpp* brute = world->SpawnActor<ABrute_cpp>(ObjectToSpawn[1], spawnLocation, rotator, SpawnParams);
	//}
}