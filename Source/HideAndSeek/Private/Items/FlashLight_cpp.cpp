// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/FlashLight_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFlashLight_cpp::AFlashLight_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

// 플레이어가 플래시 라이트와 상호작용할 때 작동할 함수.
void AFlashLight_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 플래시 라이트를 얻는 메서드를 호출함.
	Player->AddFlashLight();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 플래시 라이트를 처음 얻은 상태라면 플래시 라이트 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item2_FlashLight)
			{
				SaveData->CollectArchives.Item2_FlashLight = true;
				Player->SetArchiveGetText(NSLOCTEXT("AFlashLight_cpp", "Get_FlashLight", "Flashlight\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void AFlashLight_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}