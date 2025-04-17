// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ComponentAction/Records.h"
#include "MyStoveSDKObject.h"
//#include "Interfaces/OnlineSessionInterface.h"
//#include "Interfaces/OnlineUserCloudInterface.h"
//#include "Interfaces/OnlineIdentityInterface.h"
#include "HorrorGameGameInstance.generated.h"

struct FHorrorGameItemData;
class UDataTable;
UCLASS()
class HIDEANDSEEK_API UHorrorGameGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UHorrorGameGameInstance();

	virtual void OnStart() override;
	virtual void Init() override;

	/*void GetOnlineSubsystem();

	UFUNCTION(BlueprintCallable)
	bool WriteUserFile_Test(const FString& FileName, const FString& FileContents, bool bCompressBeforeUpload);

	IOnlineSessionPtr OnlineSessionInterface;
	IOnlineUserCloudPtr OnlineCloudInterface;
	IOnlineIdentityPtr OnlineIdentityInterface;

	FString Filename;*/


	UFUNCTION()
		virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
		virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	FHorrorGameItemData* GetHorrorGameItemData(int32 Name);

	// void ShowStageSelectWidget(bool bIsShow);
	void StopSound();

	UPROPERTY()
		UMyStoveSDKObject* _stoveSDKObject;

	UFUNCTION(BlueprintCallable)
		bool ClearedChapterSaveLogic(int32 inClearedChapter);

	UFUNCTION(BlueprintCallable)
		bool BrightGammaSaveLogic(float inBrightGamma);

	UFUNCTION(BlueprintCallable)
		bool MotionBlurSaveLogic(bool inMotionBlur);

	UFUNCTION(BlueprintCallable)
		bool VolumeSaveLogic(float inVolume);

	UFUNCTION(BlueprintCallable)
		bool MouseSensitiveSaveLogic(float inMouseSensitive);

	UFUNCTION(BlueprintCallable)
		bool CrossHairOnSaveLogic(bool inIsCrossHairOn);

	UFUNCTION(BlueprintCallable)
	bool TimerOnSaveLogic(bool inIsTimerOn);

	UFUNCTION(BlueprintCallable)
	bool OptionSettingSaveLogic(FOptionSettings inOptionSetting);

	UFUNCTION(BlueprintCallable)
	bool CharacteristicsOnSaveLogic();

	UFUNCTION(BlueprintCallable)
	bool TipsOnSaveLogic();

	UFUNCTION(BlueprintCallable)
	int32 GetClearedChapter();

	UFUNCTION(BlueprintCallable)
	float GetBrightGamma();

	UFUNCTION(BlueprintCallable)
	void ApplyBrightness();

	UFUNCTION(BlueprintCallable)
	bool GetMotionBlur();

	UFUNCTION(BlueprintCallable)
	float GetVolume();

	UFUNCTION(BlueprintCallable)
	float GetMouseSensitive();

	UFUNCTION(BlueprintCallable)
	bool GetIsCrossHairOn();

	UFUNCTION(BlueprintCallable)
	bool GetIsTimerOn();

	UFUNCTION(BlueprintCallable)
	ELanguage GetCurrentLanguage();

	UFUNCTION(BlueprintCallable)
	bool ClearTimeSaveLogic(int32 inClearedChapter, FString inClearTime); // inClearedChapter는 클리어한 챕터, inClearTime은 클리어 시의 시간

	UFUNCTION(BlueprintCallable)
	FText GetClearTime(int32 inClearedChapter);

	UFUNCTION(BlueprintCallable)
	FString GetCurrentClearTime();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetAllClearedTime();

	UFUNCTION(BlueprintCallable)
	TArray<FClearData> GetAllClearData();

	UFUNCTION(BlueprintCallable)
	bool ChapterClearSaveLogic(int32 inClearedChapter, float inClearTime, int32 inNextChapter);

	// 아이템을 처음 획득한 것인지 알아보는 로직.
	UFUNCTION(BlueprintCallable)
	bool GetIsItemFirstAcquired(const EItemNumber ItemNumber);

	// 처음 획득한 아이템의 문서를 저장하는 로직.
	UFUNCTION(BlueprintCallable)
	void SaveItemArchives(const EItemNumber ItemNumber);

	// 요괴에게 처음 죽은 것인지 알아보는 로직.
	UFUNCTION(BlueprintCallable)
	bool GetIsYokaiFirstDied(const EYokaiType YokaiType);

	// 요괴의 문서를 저장하는 로직.
	UFUNCTION(BlueprintCallable)
	void SaveYokaiArchives(const EYokaiType YokaiType);

	UFUNCTION(BlueprintCallable)
	UHorrorGameSaveGame* GetSaveData();

	// This function will be called, when player selects difficulty at chapter select widget.
	UFUNCTION(BlueprintCallable)
	void SetLevelDifficulty(int32 inSelectedDifficulty);

	// Returns the number of objects to collect depending on the difficulty level
	UFUNCTION(BlueprintCallable)
	int GetObjectCountToCollect() const;

	// Get the HP Loss rate
	UFUNCTION(BlueprintCallable)
	float GetHPLossRate() const;

	// Public Variables
public:
	// 레벨 난이도. 0: Easy, 1: Normal(default), 2: Hard, 3: Nightmare
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Chapter)
	int32 LevelDifficulty;

private:
	UPROPERTY()
	UDataTable* HorrorGameTable;

	UPROPERTY()
	TObjectPtr<class UHorrorGameSaveGame> SaveData;

	UPROPERTY()
		class UUserWidget* LoadingWidget;
	TSubclassOf<UUserWidget>WidgetClass;

	UUserWidget* CurrentWidget;

	class USoundCue* OpeningCue;
	class UAudioComponent* BGM_Comp;

	UPROPERTY()
	int32 ClearedChaptersNumber;

	UPROPERTY()
		FString CurrentClearedTime;

	UPROPERTY()
		TArray<FString> ClearedTime;

	UPROPERTY()
		FOptionSettings OptionSetting;

	//UPROPERTY()
	//	float BrightGamma; // 밝기 감마. (하한 0.5 ~ 상한 5.0)

	//UPROPERTY()
	//	bool bMotionBlur; // 모션 블러 설정

	//UPROPERTY()
	//	float Volume; // 볼륨 설정

	//UPROPERTY()
	//	float MouseSensitive; // 마우스 감도 설정

	//UPROPERTY()
	//	bool bIsCrossHairOn; // 크로스헤어 설정

	//UPROPERTY()
	//	bool bIsTimerOn; // 타이머 설정

	UPROPERTY()
		TArray<FClearData> ClearChapter;
};
