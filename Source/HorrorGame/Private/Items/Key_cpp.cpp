// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Key_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"

// Sets default values
AKey_cpp::AKey_cpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	FVector DefaultLocation = FVector(0.0f, 0.0f, 0.0f);
	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key1"));
	KeyMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeySM(TEXT("/Game/Assets/Items/Key"));
	if (KeySM.Succeeded())
	{
		KeyMesh->SetStaticMesh(KeySM.Object);
		KeyMesh->SetRelativeLocation(DefaultLocation);
		KeyMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	}
	KeyMesh->SetCollisionProfileName("ItemObjects");
}

void AKey_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	//UE_LOG(LogTemp, Warning, TEXT("Interact"));
	Player->AddKey();
	if (Player->bCanItemGet)
	{
		Destroy();
	}
}

void AKey_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}