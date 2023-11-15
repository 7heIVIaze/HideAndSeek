// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ComponentAction/HorrorGameSaveGame.h"

UHorrorGameSaveGame::UHorrorGameSaveGame()
{
	bLevelCleared.Init(false, 3);
	bIsItemGet.Init(false, 10);
	bIsCatchedByCreature.Init(false, 3);
}