// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "Items/Cutter_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"

// Sets default values
ACutter_cpp::ACutter_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultLoc = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	DefaultSceneRoot->SetWorldLocation(DefaultLoc);
	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
	RootComponent = DefaultSceneRoot;

	CutterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cutter"));
	CutterMesh->SetupAttachment(DefaultSceneRoot);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>CutterObj(TEXT("/Game/Assets/Items/bolt_cutters"));
	if (CutterObj.Succeeded())
	{
		CutterMesh->SetStaticMesh(CutterObj.Object);
	}
	CutterMesh->SetRelativeLocation(DefaultLoc);
	CutterMesh->SetRelativeScale3D(DefaultScale * 0.7f);
	CutterMesh->SetCollisionProfileName("ItemObjects");

}

void ACutter_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);
	Player->AddCutter();
	if (Player->bCanItemGet)
	{
		Player->CutterDurability = Durability;
		Destroy();
	}
}

void ACutter_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}