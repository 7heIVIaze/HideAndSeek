// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ComponentAction/Records.h"
#include "MyStoveSDKObject.h"
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
		bool CharacteristicsOnSaveLogic();

	UFUNCTION(BlueprintCallable)
		bool TipsOnSaveLogic();

	UFUNCTION(BlueprintCallable)
		int32 GetClearedChapter();

	UFUNCTION(BlueprintCallable)
		float GetBrightGamma();

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
		bool ClearTimeSaveLogic(int32 inClearedChapter, FString inClearTime); // inClearedChapter는 클리어한 챕터, inClearTime은 클리어 시의 시간

	UFUNCTION(BlueprintCallable)
		FText GetClearTime(int32 inClearedChapter);

	UFUNCTION(BlueprintCallable)
		FString GetCurrentClearTime();

	UFUNCTION(BlueprintCallable)
		TArray<FString> GetAllClearedTime();

	UFUNCTION(BlueprintCallable)
		bool ChapterClearSaveLogic(int32 inClearedChapter, FString inClearTime);

private:
	UPROPERTY()
	UDataTable* HorrorGameTable;

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
		float BrightGamma; // 밝기 감마. (하한 0.5 ~ 상한 5.0)

	UPROPERTY()
		bool bMotionBlur; // 모션 블러 설정

	UPROPERTY()
		float Volume; // 볼륨 설정

	UPROPERTY()
		float MouseSensitive; // 마우스 감도 설정

	UPROPERTY()
		bool bIsCrossHairOn; // 크로스헤어 설정

	UPROPERTY()
		bool bIsTimerOn; // 타이머 설정

	UPROPERTY()
		TArray<FClearData> ClearChapter;
};
