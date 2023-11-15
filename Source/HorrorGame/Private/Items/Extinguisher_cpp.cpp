// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Extinguisher_cpp.h"

// Sets default values
AExtinguisher_cpp::AExtinguisher_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	ExtinguisherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Extinguisher"));
	ExtinguisherMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>ExtinguisherObj(TEXT("/Game/Assets/Items/FireExtinguisher"));
	if (ExtinguisherObj.Succeeded())
	{
		ExtinguisherMesh->SetStaticMesh(ExtinguisherObj.Object);
	}
	ExtinguisherMesh->SetRelativeLocation(DefaultPos);
	ExtinguisherMesh->SetRelativeScale3D(FVector(2.4f, 2.4f, 2.0f));
	ExtinguisherMesh->SetCollisionProfileName("ItemObjects");
}

// Called when the game starts or when spawned
void AExtinguisher_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddExtinguisher();
	if (Player->bCanItemGet)
	{
		Destroy();
	}
}

void AExtinguisher_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}