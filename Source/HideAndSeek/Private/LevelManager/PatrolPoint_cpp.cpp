// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LevelManager/PatrolPoint_cpp.h"
#include "Components/BoxComponent.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "LevelManager/LevelManager.h"
#include "EngineUtils.h"

// Sets default values
APatrolPoint_cpp::APatrolPoint_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
}

// Called when the game starts or when spawned
void APatrolPoint_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 레벨 매니저에게 자신을 순찰지점 리스트에 추가하도록 함.
	for (TActorIterator<ALevelManager>LevelManager(GetWorld()); LevelManager; ++LevelManager)
	{
		ALevelManager* CurrentLevelManager = *LevelManager;
		CurrentLevelManager->PatrolPoint.Add(true);
	}
}
