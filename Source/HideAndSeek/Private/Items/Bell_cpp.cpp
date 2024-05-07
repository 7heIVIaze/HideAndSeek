// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Bell_cpp.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Altar_cpp.h"

// Sets default values
ABell_cpp::ABell_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void ABell_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddBell();
	if (Player->bCanItemGet)
	{
		//USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ObjectGet"));
		if (ObjectSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
		}
		Altar->UnSealedObjectNumber(1);
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item6_BronzeBell) // 청동 방울을 처음 얻은 상태라면
			{
				SaveData->CollectArchives.Item6_BronzeBell = true;
				Player->SetArchiveGetText(NSLOCTEXT("ABell_cpp", "Get_Bell", "Bronze Bell\nis added in archive"));
				SaveData->SaveData();
			}
		}
		Destroy();
	}
}

void ABell_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}