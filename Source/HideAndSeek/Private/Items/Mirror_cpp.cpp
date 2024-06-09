// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Mirror_cpp.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Runner_cpp.h"
#include "Altar_cpp.h"

// Sets default values
AMirror_cpp::AMirror_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(FVector::ZeroVector);
	RootComponent = DefaultSceneRoot;

	MirrorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mirror"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MirrorModel(TEXT("/Game/Assets/Items/BronzeMirror"));
	if (MirrorModel.Succeeded())
	{
		MirrorMesh->SetStaticMesh(MirrorModel.Object);
		MirrorMesh->SetRelativeLocation(FVector::ZeroVector);
		MirrorMesh->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
		MirrorMesh->SetRelativeScale3D(FVector::OneVector * 5.f);
	}
	MirrorMesh->SetCollisionProfileName(TEXT("ItemObjects"));
	MirrorMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AMirror_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에 배치된 제단 액터를 찾아서 설정함.
	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

// 플레이어가 청동 거울 아이템을 습득하려 할 때 작동할 함수.
void AMirror_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 거울을 얻는 메서드를 호출함.
	Player->AddMirror();

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
		
		// 청동 거울을 처음 얻은 상태라면 청동 거울 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item7_BronzeMirror)
			{
				SaveData->CollectArchives.Item7_BronzeMirror = true;
				Player->SetArchiveGetText(NSLOCTEXT("AMirror_cpp", "Get_Mirror", "Bronze Mirror\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

// 플레이어가 청동 방울 아이템을 사용하려 할 때 작동할 함수.
void AMirror_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	UWorld* World = GetWorld();
	
	// 월드에 배치된 요괴 개체(리퍼, 브루트, 러너)의 시간(틱 업데이트)을 느리게 만듦.
	for (TActorIterator<AReaper_cpp>entity(World); entity; ++entity)
	{
		AReaper_cpp* Reaper = *entity;
		Reaper->SetStun();
	}

	for (TActorIterator<ABrute_cpp>entity(World); entity; ++entity)
	{
		ABrute_cpp* Brute = *entity;
		Brute->SetStun();
	}

	for (TActorIterator<ARunner_cpp>entity(World); entity; ++entity)
	{
		ARunner_cpp* Runner = *entity;
		Runner->SetStun();
	}

	// 그 후 플레이어가 소유한 거울 개수 감소.
	Player->MirrorCount--;
}