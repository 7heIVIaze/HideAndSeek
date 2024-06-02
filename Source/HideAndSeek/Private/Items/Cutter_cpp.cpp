// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Cutter_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

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
	Player->CutterDurability = Durability;
	Player->AddCutter();
	if (Player->bCanItemGet)
	{
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item9_Cutter) // 절단기를 처음 얻은 상태라면
			{
				SaveData->CollectArchives.Item9_Cutter = true;
				Player->SetArchiveGetText(NSLOCTEXT("ACutter_cpp", "Get_Cutter", "Cutter\nis added in archive"));
				SaveData->SaveData();
			}
		}
		//Player->GameUIWidget->SetCutterHUD(Durability);
		Destroy();
	}
}

void ACutter_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}