// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Bell_cpp.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Furniture/Altar_cpp.h"

// Sets default values
ABell_cpp::ABell_cpp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 메시들의 기본 설정을 해줌. (세세한 설정은 블루프린트 클래스에서 수행)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BellMesh(TEXT("/Game/Assets/Items/bell"));
	if (BellMesh.Succeeded())
	{
		ItemMesh->SetStaticMesh(BellMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABell_cpp::BeginPlay()
{
	Super::BeginPlay();
	
	// 월드에 배치된 제단 액터를 찾아서 설정함.
	UWorld* World = GetWorld();
	for (TActorIterator<AAltar_cpp>entity(World); entity; ++entity)
	{
		Altar = *entity;
	}
}

// 플레이어가 청동 방울 아이템을 습득하려 할 때 작동할 함수.
void ABell_cpp::OnInteract(class AHorrorGameCharacter* Player)
{
	Super::OnInteract(Player);

}

bool ABell_cpp::UseInteract(class AHorrorGameCharacter* Player)
{
	Super::UseInteract(Player);
	return true;
}