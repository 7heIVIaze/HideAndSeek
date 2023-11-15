// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/FlashLight_cpp.h"

// Sets default values
AFlashLight_cpp::AFlashLight_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultPos);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	FlashLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashLight"));
	FlashLightMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>FlashLightObj(TEXT("/Game/Assets/Items/FlashLight"));
	if (FlashLightObj.Succeeded())
	{
		FlashLightMesh->SetStaticMesh(FlashLightObj.Object);
	}
	FlashLightMesh->SetRelativeLocation(DefaultPos);
	FlashLightMesh->SetRelativeScale3D(DefaultScale * 0.2f);
	FlashLightMesh->SetCollisionProfileName("ItemObjects");
}

void AFlashLight_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddFlashLight();
	if (Player->bCanItemGet)
	{
		Destroy();
	}
}

void AFlashLight_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}