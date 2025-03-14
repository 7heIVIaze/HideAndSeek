// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/ActiveItem.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AActiveItem::AActiveItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Meshes default settings.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	MeshComponent->SetupAttachment(RootComp);

}

// Called when the game starts or when spawned
void AActiveItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AActiveItem::UseItem()
{
	return true;
}

void AActiveItem::SetupDurability(int inDurability)
{
	Durability = inDurability;
}

void AActiveItem::SetItemVisibility(bool NewVisibility)
{
	MeshComponent->SetHiddenInGame(NewVisibility);
}

//
//	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
//	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	FVector DefaultLocation = FVector(0.0f, 0.0f, 0.0f);
//	DefaultSceneRoot->SetWorldLocation(FVector(0.0f, 0.0f, 0.0f));
//	FVector DefaultScale = FVector(1.0f, 1.0f, 1.0f);
//	DefaultSceneRoot->SetWorldScale3D(DefaultScale);
//	RootComponent = DefaultSceneRoot;
//
//	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key1"));
//	KeyMesh->SetupAttachment(DefaultSceneRoot);
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeySM(TEXT("/Game/Assets/Items/Key"));
//	if (KeySM.Succeeded())
//	{
//		KeyMesh->SetStaticMesh(KeySM.Object);
//		KeyMesh->SetRelativeLocation(DefaultLocation);
//		KeyMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
//	}
//	KeyMesh->SetCollisionProfileName("ItemObjects");
//}
//
//// 플레이어가 열쇠와 상호작용할 때 작동할 함수.
//void AActiveItem::OnInteract(class AHorrorGameCharacter* Player)
//{
//	Super::OnInteract(Player);
//	
//	// 플레이어의 열쇠를 얻는 메서드를 호출함.
//	Player->AddKey();
//
//	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
//	if (Player->bCanItemGet)
//	{
//		// 열쇠를 처음 얻은 상태라면 열쇠 문서를 세이브 데이터에 영구히 저장함.
//		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
//		{
//			if (!SaveData->CollectArchives.Item3_Key)
//			{
//				SaveData->CollectArchives.Item3_Key = true;
//				Player->SetArchiveGetText(NSLOCTEXT("AActiveItem", "Get_Key", "Key\nis added in archive"));
//				SaveData->SaveData();
//			}
//		}
//
//		// 그 후 배치된 이 액터를 제거함.
//		Destroy();
//	}
//}
//
//void AActiveItem::UseInteract(class AHorrorGameCharacter* Player)
//{
//	Super::UseInteract(Player);
//}