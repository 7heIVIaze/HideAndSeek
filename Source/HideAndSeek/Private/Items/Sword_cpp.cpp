// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 아이템으로 존재할 칼임
#include "Items/Sword_cpp.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Altar_cpp.h"

// Sets default values
ASword_cpp::ASword_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector fDefaultLoc(0.0f, 0.0f, 0.0f);
	FVector fDefaultScale(1.0f, 1.0f, 1.0f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(fDefaultLoc);
	RootComponent = DefaultSceneRoot;

	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	Sword->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SwordMesh(TEXT("/Game/Assets/Items/Sword"));
	if (SwordMesh.Succeeded())
	{
		Sword->SetStaticMesh(SwordMesh.Object);
	}
	Sword->SetRelativeLocation(fDefaultLoc);
	Sword->SetRelativeScale3D(fDefaultScale);
	// Sword->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
	Sword->SetCollisionProfileName(TEXT("ItemObjects"));
}

void ASword_cpp::BeginPlay()
{
	Super::BeginPlay();

	// 월드에 배치된 제단 액터를 찾아서 설정함.
	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

// 플레이어가 청동 검 아이템을 습득하려 할 때 작동할 함수.
void ASword_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);	

	// 플레이어의 검을 얻는 메서드를 호출함.
	Player->AddSword();

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
		
		// 청동 검을 처음 얻은 상태라면 청동 검 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item5_BronzeSword)
			{
				SaveData->CollectArchives.Item5_BronzeSword = true;
				Player->SetArchiveGetText(NSLOCTEXT("ASword_cpp", "Get_Sword", "Bronze Sword\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void ASword_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

}