// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Extinguisher_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExtinguisher_cpp::AExtinguisher_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FVector DefaultPos = FVector(0.f, 0.f, 0.f);
	FVector DefaultScale = FVector(1.f, 1.f, 1.f);

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
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

// 플레이어가 소화기와 상호작용할 때 작동할 함수.
void AExtinguisher_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

	// 플레이어의 소화기를 얻는 메서드를 호출함.
	Player->AddExtinguisher();

	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
	if (Player->bCanItemGet)
	{
		// 소화기를 처음 얻은 상태라면 소화기 문서를 세이브 데이터에 영구히 저장함.
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			if (!SaveData->CollectArchives.Item8_Extinguisher)
			{
				SaveData->CollectArchives.Item8_Extinguisher = true;
				Player->SetArchiveGetText(NSLOCTEXT("AExtinguisher_cpp", "Get_Extinguisher", "Extinghuisher\nis added in archive"));
				SaveData->SaveData();
			}
		}

		// 그 후 배치된 이 액터를 제거함.
		Destroy();
	}
}

void AExtinguisher_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
}