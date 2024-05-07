// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com
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

	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

void ASword_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);	

	Player->AddSword();
	if (Player->bCanItemGet)
	{
	//	USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ObjectGet"));
		if (ObjectSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
		}
		Altar->UnSealedObjectNumber(1);
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item5_BronzeSword) // 청동 검을 처음 얻은 상태라면
			{
				SaveData->CollectArchives.Item5_BronzeSword = true;
				Player->SetArchiveGetText(NSLOCTEXT("ASword_cpp", "Get_Sword", "Bronze Sword\nis added in archive"));
				SaveData->SaveData();
			}
		}
		Destroy();
	}
}

void ASword_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);

}