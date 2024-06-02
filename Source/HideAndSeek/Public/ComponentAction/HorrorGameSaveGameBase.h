// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HorrorGameSaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UHorrorGameSaveGameBase : public USaveGame
{
	GENERATED_BODY()
	
public:
	// 초기화
	void InitSaveGameBase(const FString& inSlotName, int32 inUserIndex);

	// 저장
	void SaveData();

public:
	// 식별 정보
	FString SlotName;
	int32 UserIndex = 0;
};
