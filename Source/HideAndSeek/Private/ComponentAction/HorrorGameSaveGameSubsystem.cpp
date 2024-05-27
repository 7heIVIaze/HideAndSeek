// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "ComponentAction/HorrorGameSaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"

void UHorrorGameSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 슬롯 이름 프리픽스
	SlotNamePrefixMap.Add(UHorrorGameSaveGame::StaticClass(), TEXT("HorrorGameSaveGame"));
}

UHorrorGameSaveGameSubsystem* UHorrorGameSaveGameSubsystem::GetSaveGameSubsystem(UObject* inObject)
{
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(inObject))
	{
		return GameInstance->GetSubsystem<UHorrorGameSaveGameSubsystem>();
	}

	return nullptr;
}

UHorrorGameSaveGameBase* UHorrorGameSaveGameSubsystem::LoadDataLogic(UClass* inClass, const FString& inUniqueID, int32 inUserIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("SaveGameSubsystem Load Data Logic Called!"));
	FString* FindPrefix = SlotNamePrefixMap.Find(inClass);
	check(FindPrefix);

	FString SlotName = FString::Printf(TEXT("%s_%s"), *(*FindPrefix), *inUniqueID);

	// 찾아보고 없으면 데이터 생성
	USaveGame* Temp = Cast<UHorrorGameSaveGameBase>(UGameplayStatics::LoadGameFromSlot(SlotName, inUserIndex));
	if (nullptr == Temp)
	{
		Temp = UGameplayStatics::CreateSaveGameObject(inClass);
		UHorrorGameSaveGame* TempSaveData = Cast<UHorrorGameSaveGame>(Temp);

		TempSaveData->OptionSetting.BrightGamma = 2.2f;
		TempSaveData->OptionSetting.MouseSensitive = 1.0f; // 기본 마우스 감도 값은 1.0
		TempSaveData->OptionSetting.Volume = 1.0f; // 기본 볼륨 값은 1.0
		TempSaveData->OptionSetting.bMotionBlur = true;
		TempSaveData->OptionSetting.bIsTimerOn = true;
		TempSaveData->OptionSetting.bIsCrossHairOn = true;
		TempSaveData->OptionSetting.Language = ELanguage::LANG_En; // 기본 언어는 영어
		//TempSaveData->ClearedChapter = 0;
		TempSaveData->ClearChapter.Init(FClearData(), 5);
		for (int i = 0; i < 5; ++i)
		{
			TempSaveData->ClearChapter[i].ChapterNumber = i;
		}
		TempSaveData->ClearChapter[0].bIsOpened = true;

		TempSaveData->SaveData();
	}

	UHorrorGameSaveGameBase* SaveData = Cast<UHorrorGameSaveGameBase>(Temp);
	if (nullptr == SaveData)
	{
		return nullptr;
	}

	// 초기화
	SaveData->InitSaveGameBase(SlotName, inUserIndex);
	return Cast<UHorrorGameSaveGameBase>(SaveData);
}