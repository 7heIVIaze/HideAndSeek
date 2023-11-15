// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "LevelManager.h"
#include "EngineUtils.h"
#include "PatrolPoint_cpp.h"
#include "CreatureSpawner.h"
#include "HorrorGamePlayerController.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "HorrorGame/HorrorGameGameMode.h"
#include "HorrorGameGameInstance.h"
#include "Altar_cpp.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelManager::ALevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (count++ > 60)
	{
		for (TActorIterator<APatrolPoint_cpp>entity(World); entity; ++entity)
		{
			FString entityName = entity->GetActorLabel(); //->GetName();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *entityName);
			entityName.RemoveFromStart(TEXT("BP_PatrolPoint"));
			int32 idx = FCString::Atoi(*entityName);
			UE_LOG(LogTemp, Warning, TEXT("%d"), idx);
			if (idx > 0 || idx <= 25)
				if (!IsAllLoaded[idx - 1])
					IsAllLoaded[idx - 1] = true;
		}

		if (!bIsCalled)
		{
			if (Check())
			{
				Altar->ReaperSpawn();
				for (int i = 0; i < Spawners.Num(); ++i)
				{
					//ACreatureSpawner* Spawner = Cast<ACreatureSpawner>(*Spawners[i]);
					Spawners[i]->CreatureSpawn();
				}
				bIsCalled = true;
				Destroy();
			}
		}
		count = 0;
		
	}
}

bool ALevelManager::Check()
{
	for (int i = 0; i < 25; ++i)
	{
		if (!IsAllLoaded[i]) return false;
		UE_LOG(LogTemp, Warning, TEXT("IsAllLoaded[%d] is true"), i);
	}

	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(GetWorld()->GetFirstPlayerController());
	/*UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(GetGameInstance());
	GameInstance->EndLoadingScreen(GetWorld());*/
	PlayerController->ShowMainUI();
	Cast<AHorrorGameCharacter>(PlayerController->GetPawn())->LevelStart();
	Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StartBackGroundMusic();
	
	return true;
}
