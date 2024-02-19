// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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