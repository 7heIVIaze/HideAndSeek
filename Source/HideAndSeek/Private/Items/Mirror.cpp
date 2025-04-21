// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Items/Mirror.h"
#include "EngineUtils.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/CreatureClass.h"
#include "Furniture/Altar_cpp.h"

// Sets default values
AMirror::AMirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMirror::BeginPlay()
{
	Super::BeginPlay();
	
}
bool AMirror::UseItem()
{
	Super::UseItem();
	// 월드에 배치된 요괴 개체(리퍼, 브루트, 러너)의 시간(틱 업데이트)을 느리게 만듦.
	for (TActorIterator<ACreatureClass>entity(GetWorld()); entity; ++entity)
	{
		ACreatureClass* Yokai = *entity;
		Yokai->SetStun();
	}

	// 아직 거울 속 세계(지역)으로 순간이동한 후에 해당 지역의 다른 거울을 이용해 다른 곳으로 빠져 나오는 방식.
	return true;
}