// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HorrorGameGameInstance.h"
#include "CustomDataTables.h"
#include "HUD/StageSelectWidget.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer/Public/MoviePlayer.h"
#include "Widgets/SCompoundWidget.h"
#include "PatrolPoint_cpp.h"
#include "ComponentAction/HorrorGameSaveGame.h"

UHorrorGameGameInstance::UHorrorGameGameInstance()
{
	_stoveSDKObject = NewObject<UMyStoveSDKObject>();
	ClearedChaptersNumber = 0;
	BrightGamma = 2.2f; // 기본 감마 값은 2.2
	MouseSensitive = 1.0f; // 기본 마우스 감도 값은 1.0
	Volume = 1.0f; // 기본 볼륨 값은 1.0
	bMotionBlur = true;
	static ConstructorHelpers::FObjectFinder<USoundCue>OpeningMusicAsset(TEXT("/Game/Assets/Sounds/SoundCues/OpeningBGM"));
	if (OpeningMusicAsset.Succeeded())
	{
		OpeningCue = OpeningMusicAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>LoadingWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_LoadingWidget"));
	if (LoadingWidgetClass.Succeeded())
	{
		WidgetClass = LoadingWidgetClass.Class;
	}
}

void UHorrorGameGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UHorrorGameGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UHorrorGameGameInstance::EndLoadingScreen);

	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		ClearedChaptersNumber = SaveData->ClearChapter.Num();
		BrightGamma = SaveData->BrightGamma;
		bMotionBlur = SaveData->bMotionBlur;
		Volume = SaveData->Volume;
		MouseSensitive = SaveData->MouseSensitive;
		bIsCrossHairOn = SaveData->bIsCrossHairOn;
		bIsTimerOn = SaveData->bIsTimerOn;
		ClearedTime = SaveData->ClearTime;
		ClearChapter = SaveData->ClearChapter;
	}
}

void UHorrorGameGameInstance::OnStart()
{
	Super::OnStart();
}

FHorrorGameItemData* UHorrorGameGameInstance::GetHorrorGameItemData(int32 Name)
{
	return HorrorGameTable->FindRow<FHorrorGameItemData>(*FString::FromInt(Name), TEXT(""));
}

void UHorrorGameGameInstance::StopSound()
{
	if(BGM_Comp)
	BGM_Comp->Stop();
}

// deprecated
bool UHorrorGameGameInstance::ClearedChapterSaveLogic(int32 inClearedChapter)
{
	if (ClearedChaptersNumber < inClearedChapter) // 아직 클리어하지 않은 챕터를 클리어한 경우
	{
		ClearedChaptersNumber = inClearedChapter;
		if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
		{
			SaveData->ClearedChapter = ClearedChaptersNumber;
			SaveData->SaveData();
			return true;
		}
	}
	else // 클리어한 챕터를 다시 클리어한 경우
	{
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::BrightGammaSaveLogic(float inBrightGamma)
{
	BrightGamma = inBrightGamma;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->BrightGamma = BrightGamma;
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::MotionBlurSaveLogic(bool inMotionBlur)
{
	bMotionBlur = inMotionBlur;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->bMotionBlur = bMotionBlur;
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::VolumeSaveLogic(float inVolume)
{
	Volume = inVolume;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->Volume = Volume;
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::MouseSensitiveSaveLogic(float inMouseSensitive)
{
	MouseSensitive = inMouseSensitive;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->MouseSensitive = MouseSensitive;
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::CrossHairOnSaveLogic(bool inIsCrossHairOn)
{
	bIsCrossHairOn = inIsCrossHairOn;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->bIsCrossHairOn = bIsCrossHairOn;
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::TimerOnSaveLogic(bool inIsTimerOn)
{
	bIsTimerOn = inIsTimerOn;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->bIsTimerOn = bIsTimerOn;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// deprecated
bool UHorrorGameGameInstance::ClearTimeSaveLogic(int32 inClearedChapter, FString inClearTime)
{
	CurrentClearedTime = inClearTime;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if (SaveData->ClearTime.Num() >= ClearedChaptersNumber) // 만약 이미 클리어한 챕터인 경우
		{
			if (SaveData->ClearTime[inClearedChapter - 1] > inClearTime) // 만약 기록 갱신에 성공한 경우
			{
				SaveData->ClearTime[inClearedChapter - 1] = inClearTime; // 기록 갱신
			}
		}
		else // 처음 클리어하는 거라면
		{
			SaveData->ClearTime.Add(inClearTime); // 갱신
		}
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::ChapterClearSaveLogic(int32 inClearedChapter, FString inClearTime)
{
	//CurrentClearedTime = inClearTime;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		bool bCanUpdate = true; // 갱신이 가능한지(처음 저장 또는 갱신)
		FString ClearTime = inClearTime;
		if (ClearTime.Len() == 8)
		{
			ClearTime = TEXT("00:") + ClearTime;
		}

		for (int i = 0; i < SaveData->ClearChapter.Num(); ++i)
		{
			if (SaveData->ClearChapter[i].ChapterNumber == inClearedChapter)
			{
				if (SaveData->ClearChapter[i].bIsCleared)
				{
					if (SaveData->ClearChapter[i].ClearTime > ClearTime) // 이전 기록보다 더 짧은 기록을 냈다면
					{
						SaveData->ClearChapter[i].ClearTime = ClearTime; // 기록 갱신
						bCanUpdate = false;
					}
					else // 짧은 기록이 아니라면
					{
						bCanUpdate = false; // 갱신 안 되게 설정
					}
				}
			}
		}
		if (bCanUpdate) // 처음 클리어하는 거라면
		{ 
			FClearData newClearData;
			newClearData.bIsCleared = true;
			newClearData.ClearTime = ClearTime;
			SaveData->ClearChapter.Emplace(newClearData);
			SaveData->ClearTime.Add(ClearTime); // 갱신
		}
		SaveData->SaveData();
		return true;
	}
	return false;
}

bool UHorrorGameGameInstance::CharacteristicsOnSaveLogic()
{
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if(!SaveData->SeeCharacteristic)
		{
			SaveData->SeeCharacteristic = true;
			SaveData->SaveData();
			return true;
		}
	}
	return false;
}

bool UHorrorGameGameInstance::TipsOnSaveLogic()
{
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if (!SaveData->SeeHowToEscape)
		{
			SaveData->SeeHowToEscape = true;
			SaveData->SaveData();
			return true;
		}
	}
	return false;
}

int32 UHorrorGameGameInstance::GetClearedChapter()
{
	return ClearedChaptersNumber;
}

float UHorrorGameGameInstance::GetBrightGamma()
{
	return BrightGamma;
}

bool UHorrorGameGameInstance::GetMotionBlur()
{
	return bMotionBlur;
}

float UHorrorGameGameInstance::GetVolume()
{
	return Volume;
}

float UHorrorGameGameInstance::GetMouseSensitive()
{
	return MouseSensitive;
}

bool UHorrorGameGameInstance::GetIsCrossHairOn()
{
	return bIsCrossHairOn;
}

bool UHorrorGameGameInstance::GetIsTimerOn()
{
	return bIsTimerOn;
}

FText UHorrorGameGameInstance::GetClearTime(int32 inClearedChapter)
{
	FText ReturnText = FText::FromString(ClearedTime[inClearedChapter]);
	return ReturnText;
}

FString UHorrorGameGameInstance::GetCurrentClearTime()
{
	return CurrentClearedTime;
}

void UHorrorGameGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		if (MapName.Contains(TEXT("Prologue")))
		{
			FLoadingScreenAttributes LoadingScreen;
			LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
			// LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
			LoadingWidget = CreateWidget(this, WidgetClass);
			LoadingScreen.WidgetLoadingScreen = LoadingWidget->TakeWidget();

			GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		}

	}
}

void UHorrorGameGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}

TArray<FString> UHorrorGameGameInstance::GetAllClearedTime()
{
	return ClearedTime;
}