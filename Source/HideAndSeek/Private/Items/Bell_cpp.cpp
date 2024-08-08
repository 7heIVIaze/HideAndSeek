// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Bell_cpp.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Furniture/Altar_cpp.h"

// Sets default values
ABell_cpp::ABell_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector::ZeroVector);
	RootComponent = DefaultSceneRoot;

	Bell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BellMesh(TEXT("/Game/Assets/Items/bell"));
	if (BellMesh.Succeeded())
	{
		Bell->SetStaticMesh(BellMesh.Object);
		Bell->SetRelativeLocation(FVector::ZeroVector);
	}

	Bell->SetCollisionProfileName(TEXT("ItemObjects"));
	Bell->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void ABell_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에 배치된 제단 액터를 찾아서 설정함.
	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

//// Called every frame
//void ABell_cpp::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// 플레이어가 청동 방울 아이템을 습득하려 할 때 작동할 함수.
void ABell_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 방울을 얻는 메서드를 호출함.
	Player->AddBell();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 오브젝트를 얻는 소리를 재생함.
		if (ObjectSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
		}

		// 제단에 봉인이 해제된 아이템 개수를 1개 추가함.
		Altar->UnSealedObjectNumber(1);

		// 청동 방울을 처음 얻은 상태라면 청동 방울 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item6_BronzeBell) 
			{
				SaveData->CollectArchives.Item6_BronzeBell = true;
				Player->SetArchiveGetText(NSLOCTEXT("ABell_cpp", "Get_Bell", "Bronze Bell\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void ABell_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}