// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Timer_cpp.h"
#include "Player/HorrorGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"

// Sets default values
ATimer_cpp::ATimer_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultLoc = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultLoc);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	TimerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Timer"));
	TimerMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TimerObj(TEXT("/Game/Assets/Items/timer"));
	if (TimerObj.Succeeded())
	{
		TimerMesh->SetStaticMesh(TimerObj.Object);
	}
	TimerMesh->SetRelativeLocation(DefaultLoc);
	TimerMesh->SetRelativeScale3D(FVector(3.f, 2.f, 3.f));
	TimerMesh->SetCollisionProfileName("ItemObjects");
}

// Called when the game starts or when spawned
void ATimer_cpp::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
//void ATimer_cpp::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// 플레이어가 타이머 아이템을 습득하려 할 때 작동할 함수.
void ATimer_cpp::OnInteract(class AHorrorGameCharacter* Player) // Player Click Event
{
	Super::OnInteract(Player);

	// 플레이어의 타이머를 얻는 메서드를 호출함.
	Player->AddTimer();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 타이머를 처음 얻은 상태라면 타이머 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item4_Timer)
			{
				SaveData->CollectArchives.Item4_Timer = true;
				Player->SetArchiveGetText(NSLOCTEXT("ATimer_cpp", "Get_Timer", "Timer\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void ATimer_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}