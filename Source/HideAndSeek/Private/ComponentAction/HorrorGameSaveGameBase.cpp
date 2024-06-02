// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

// 세이브 데이터를 관리할 클래스로 루트 클래스임.
#include "ComponentAction/HorrorGameSaveGameBase.h"
#include "Kismet/GameplayStatics.h"

void UHorrorGameSaveGameBase::InitSaveGameBase(const FString& inSlotName, int32 inUserIndex)
{
	SlotName = inSlotName;
	UserIndex = inUserIndex;
}

void UHorrorGameSaveGameBase::SaveData()
{
	UGameplayStatics::SaveGameToSlot(this, SlotName, UserIndex);
}