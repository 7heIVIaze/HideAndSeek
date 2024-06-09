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

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

// 플레이어가 절단기와 상호작용할 때 작동할 함수.
void ACutter_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 절단기를 얻는 메서드를 호출함.
	Player->AddCutter();
	
	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상황이면
	if (Player->bCanItemGet)
	{
		// 절단기의 남은 내구도를 이 절단기의 내구도로 복구함.
		Player->CutterDurability = Durability;

		// 절단기를 처음 얻은 상태라면 절단기 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item9_Cutter)
			{
				SaveData->CollectArchives.Item9_Cutter = true;
				Player->SetArchiveGetText(NSLOCTEXT("ACutter_cpp", "Get_Cutter", "Cutter\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void ACutter_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}