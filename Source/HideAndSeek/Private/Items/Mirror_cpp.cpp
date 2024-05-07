// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
	
	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

void AMirror_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddMirror();
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
			if (!SaveData->CollectArchives.Item7_BronzeMirror) // 청동 거울을 처음 얻은 상태라면
			{
				SaveData->CollectArchives.Item7_BronzeMirror = true;
				Player->SetArchiveGetText(NSLOCTEXT("AMirror_cpp", "Get_Mirror", "Bronze Mirror\nis added in archive"));
				SaveData->SaveData();
			}
		}
		Destroy();
	}
}

void AMirror_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

	UWorld* World = GetWorld();

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

	Player->MirrorCount--;
}