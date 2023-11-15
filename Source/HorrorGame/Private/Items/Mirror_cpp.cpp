// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Mirror_cpp.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/Reaper_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Runner_cpp.h"

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
	
}

// Called every frame
//void AMirror_cpp::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//

void AMirror_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	USoundCue* ObjectSound = LoadObject<USoundCue>(nullptr, TEXT("/Game/Assets/Sounds/SoundCues/ObjectGet"));
	if (ObjectSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
	}
	Player->AddMirror();
	if (Player->bCanItemGet)
	{
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