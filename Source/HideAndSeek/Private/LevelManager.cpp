// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LevelManager.h"
#include "EngineUtils.h"
#include "PatrolPoint_cpp.h"
#include "CreatureSpawner.h"
#include "Player/PlayerSpawner.h"
#include "HorrorGamePlayerController.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "HideAndSeek/HorrorGameGameMode.h"
#include "HorrorGameGameInstance.h"
#include "Altar_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/ItemManager.h"

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

	// 60 프레임마다 실행되도록 함.
	if (count++ > 60)
	{
		// 아직 호출되지 않았을 경우
		if (!bIsCalled)
		{
			// 레벨 시작 조건을 체크함.
			if (Check())
			{
				// 아이템 매니저 클래스를 월드에서 하나씩 찾아서 아이템을 배치함.
				for (TActorIterator<AItemManager>entity(World); entity; ++entity)
				{
					(*entity)->ItemSetting();
				}

				// 제단에서는 리퍼를 스폰시키도록 함.
				Altar->ReaperSpawn();

				// 요괴 스포너에서는 요괴들을 생성하도록 함.
				for (int i = 0; i < Spawners.Num(); ++i)
				{
					Spawners[i]->CreatureSpawn();
				}

				// 그리고 호출되었다고 설정하고
				bIsCalled = true;
				
				// 이 액터를 제거함.
				Destroy();
			}
		}

		// 60 프레임마다 검사하도록 카운터를 0으로 설정함.
		count = 0;
		
	}
}

// 레벨 시작 조건을 체크하는 함수.
bool ALevelManager::Check()
{
	// 순찰 지점은 25개이기에 25개 미만이면 false로 리턴.
	if (PatrolPoint.Num() < 25)
	{
		return false;
	}

	// 플레이어 컨트롤러 클래스를 가져와 레벨이 시작되었다고 알리고, 메인 UI도 호출시키며 BGM을 재생함.
	AHorrorGamePlayerController* PlayerController = Cast<AHorrorGamePlayerController>(GetWorld()->GetFirstPlayerController());
	Cast<AHorrorGameCharacter>(PlayerController->GetPawn())->LevelStart();
	PlayerController->ShowMainUI();
	Cast<AHorrorGameGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->StartBackGroundMusic();
	
	return true;
}