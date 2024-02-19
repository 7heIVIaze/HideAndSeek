// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ComponentAction/HorrorGameSaveGame.h"
#include "ComponentAction/HorrorGameSaveGameSubsystem.h"

UHorrorGameSaveGame* UHorrorGameSaveGame::LoadObject(UObject* inObject, const FString& inUniqueID, int32 inUserIndex)
{
	return UHorrorGameSaveGameSubsystem::LoadData<UHorrorGameSaveGame>(inObject, inUniqueID, inUserIndex);
}