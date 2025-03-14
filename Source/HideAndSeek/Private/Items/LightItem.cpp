// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/LightItem.h"
#include "Components/LightComponent.h"
#include "Components/AudioComponent.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/Altar_cpp.h"


// Sets default values
ALightItem::ALightItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightComponent = nullptr;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	bIsLightOn = false;
}

// Called when the game starts or when spawned
void ALightItem::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->SetActive(false);
}

// Called every frame
void ALightItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALightItem::TurnOn()
{
	if (AudioComponent)
	{
		AudioComponent->Play();
	}

	return true;
}

void ALightItem::LightOn()
{
	bool result = false, bWasLightOn = bIsLightOn;
	if (LightComponent)
	{
		LightComponent->SetVisibility(true);
		bIsLightOn = true;
		Timer = 0.0f;
	}
}

bool ALightItem::TurnOff()
{
	bool result = false, bWasLightOn = bIsLightOn;
	if (LightComponent)
	{
		LightComponent->SetVisibility(false);
		LightComponent->SetIntensity(LightIntensity);
		
		if (TurnoffSoundCue)
		{
			UGameplayStatics::PlaySound2D(this, TurnoffSoundCue);
		}

		bIsLightOn = false;
		Timer = 0.0f;
		return (bWasLightOn != bIsLightOn) ? true : false;
	}

	return false;
}

void ALightItem::SetupDurability(int inDurability)
{
	Durability = inDurability;
}

void ALightItem::SetItemVisibility(bool NewVisibility)
{
	LightComponent->SetHiddenInGame(NewVisibility);
}

//
//// Sets default values
//ALightItem::ALightItem()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//	FVector fDefaultLoc(0.0f, 0.0f, 0.0f);
//	FVector fDefaultScale(1.0f, 1.0f, 1.0f);
//
//	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
//	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
//	DefaultSceneRoot->SetWorldLocation(fDefaultLoc);
//	RootComponent = DefaultSceneRoot;
//
//	Sword = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
//	Sword->SetupAttachment(DefaultSceneRoot);
//	static ConstructorHelpers::FObjectFinder<UStaticMesh>SwordMesh(TEXT("/Game/Assets/Items/Sword"));
//	if (SwordMesh.Succeeded())
//	{
//		Sword->SetStaticMesh(SwordMesh.Object);
//	}
//	Sword->SetRelativeLocation(fDefaultLoc);
//	Sword->SetRelativeScale3D(fDefaultScale);
//	// Sword->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
//	Sword->SetCollisionProfileName(TEXT("ItemObjects"));
//}
//
//void ALightItem::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// 월드에 배치된 제단 액터를 찾아서 설정함.
//	UWorld* World = GetWorld();
//	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
//	{
//		Altar = *entity;
//	}
//}
//
//// 플레이어가 청동 검 아이템을 습득하려 할 때 작동할 함수.
//void ALightItem::OnInteract(class AHorrorGameCharacter* Player)
//{
//	Super::OnInteract(Player);	
//
//	// 플레이어의 검을 얻는 메서드를 호출함.
//	Player->AddSword();
//
//	// 위 메서드를 통해 플레이어가 아이템을 얻을 수 있는 상태이면
//	if (Player->bCanItemGet)
//	{
//		// 오브젝트를 얻는 소리를 재생함.
//		if (ObjectSound)
//		{
//			UGameplayStatics::PlaySoundAtLocation(this, ObjectSound, GetActorLocation());
//		}
//
//		// 제단에 봉인이 해제된 아이템 개수를 1개 추가함.
//		if (Altar)
//		{
//			Altar->UnSealedObjectNumber(1);
//		}
//		
//		// 청동 검을 처음 얻은 상태라면 청동 검 문서를 세이브 데이터에 영구히 저장함.
//		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
//		{
//			if (!SaveData->CollectArchives.Item5_BronzeSword)
//			{
//				SaveData->CollectArchives.Item5_BronzeSword = true;
//				Player->SetArchiveGetText(NSLOCTEXT("ALightItem", "Get_Sword", "Bronze Sword\nis added in archive"));
//				SaveData->SaveData();
//			}
//		}
//
//		// 그 후 배치된 이 액터를 제거함.
//		Destroy();
//	}
//}
//
//void ALightItem::UseInteract(class AHorrorGameCharacter* Player)
//{
//	Super::UseInteract(Player);
//
//}