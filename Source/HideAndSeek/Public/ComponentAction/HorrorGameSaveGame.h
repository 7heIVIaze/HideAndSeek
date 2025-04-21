// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentAction/HorrorGameSaveGameBase.h"
#include "ComponentAction/Records.h"
#include "ComponentAction/CollectArchives.h"
#include "HorrorGameSaveGame.generated.h"

/**
 * 
 */

UCLASS()
class HIDEANDSEEK_API UHorrorGameSaveGame : public UHorrorGameSaveGameBase
{
	GENERATED_BODY()
	
public:
	static UHorrorGameSaveGame* LoadObject(UObject* inObject, const FString& inUniqueID, int32 inUserIndex);

public:
	UPROPERTY()
		int32 ClearedChapter; // 클리어한 레벨

	UPROPERTY()
		TArray<FString>ClearTime; // 클리어 타임

	UPROPERTY(BlueprintReadWrite)
		TArray<FClearData>ClearChapter;

	UPROPERTY(BlueprintReadWrite)
		FOptionSettings OptionSetting;

	UPROPERTY(BlueprintReadWrite)
		FCollectedArchives CollectArchives;
};