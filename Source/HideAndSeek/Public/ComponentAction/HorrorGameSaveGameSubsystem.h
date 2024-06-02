// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentAction/HorrorGameSaveGameBase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ComponentAction/Records.h"
#include "HorrorGameSaveGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UHorrorGameSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 세이브 파일을 로드함
	template<typename T>
	static T* LoadData(UObject* inObject, const FString& inUniqueID, int32 inUserIndex)
	{
		if (UHorrorGameSaveGameSubsystem* Subsystem = GetSaveGameSubsystem(inObject))
		{
			return Cast<T>(Subsystem->LoadDataLogic(T::StaticClass(), inUniqueID, inUserIndex));
		}

		return nullptr;
	}

protected:
	// 서브 시스템을 얻음
	static UHorrorGameSaveGameSubsystem* GetSaveGameSubsystem(UObject* inObject);

	// 데이터 로드 로직
	UHorrorGameSaveGameBase* LoadDataLogic(UClass* inClass, const FString& inUniqueID, int32 inUserIndex);

public:
	// 프리픽스 맵
	UPROPERTY()
		TMap<UClass*, FString>SlotNamePrefixMap;
};
