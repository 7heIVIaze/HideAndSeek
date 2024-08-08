// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "LevelManager/HorrorGameGameInstance.h"
#include "Resource/CustomDataTables.h"
#include "HUD/StageSelectWidget.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MoviePlayer/Public/MoviePlayer.h"
#include "Widgets/SCompoundWidget.h"
#include "LevelManager/PatrolPoint_cpp.h"
#include "ComponentAction/HorrorGameSaveGame.h"
//#include "OnlineSubsystem.h"
//#include "SteamNetDriver.h"
//#include "OnlineSubsystemSteam.h"
//#include "Serialization/ArchiveSaveCompressedProxy.h"

// 생성자
UHorrorGameGameInstance::UHorrorGameGameInstance()
{
	// 설정 및 SDK 초기화
	_stoveSDKObject = NewObject<UMyStoveSDKObject>();
	ClearedChaptersNumber = 0;
	OptionSetting.BrightGamma = 2.2f;
	OptionSetting.MouseSensitive = 1.0f; // 기본 마우스 감도 값은 1.0
	OptionSetting.Volume = 1.0f; // 기본 볼륨 값은 1.0
	OptionSetting.bMotionBlur = true;
	OptionSetting.bIsTimerOn = true;
	OptionSetting.bIsCrossHairOn = true;
	OptionSetting.Language = ELanguage::LANG_En; // 기본 언어는 영어
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

	//GetOnlineSubsystem();
	// 맵이 로딩 되기 전까지 로딩화면을 출력하도록 함.(근데 생각하던거랑 달라서 필요없는 함수가 됨,)
	UE_LOG(LogTemp, Warning, TEXT("HorrorGameGameInstance Init!"));
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UHorrorGameGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UHorrorGameGameInstance::EndLoadingScreen);

	// 기존에 저장된 설정값들을 가져옴.
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		ClearedChaptersNumber = SaveData->ClearChapter.Num();
		OptionSetting.BrightGamma = SaveData->OptionSetting.BrightGamma;
		OptionSetting.bMotionBlur = SaveData->OptionSetting.bMotionBlur;
		OptionSetting.Volume = SaveData->OptionSetting.Volume;
		OptionSetting.MouseSensitive = SaveData->OptionSetting.MouseSensitive;
		OptionSetting.bIsCrossHairOn = SaveData->OptionSetting.bIsCrossHairOn;
		OptionSetting.bIsTimerOn = SaveData->OptionSetting.bIsTimerOn;
		OptionSetting.Language = SaveData->OptionSetting.Language;
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

// deprecated. 과거에 작성했던 코드.
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

// 사용자 설정 값들을 저장하는 로직.
bool UHorrorGameGameInstance::OptionSettingSaveLogic(FOptionSettings inOptionSetting)
{
	// 옵션 값을 저장한 후 세이브 데이터에 영구히 저장함.
	OptionSetting = inOptionSetting;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting = OptionSetting;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 밝기 설정 값만 저장하는 로직.
bool UHorrorGameGameInstance::BrightGammaSaveLogic(float inBrightGamma)
{
	// 밝기 설정 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.BrightGamma = inBrightGamma;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.BrightGamma = OptionSetting.BrightGamma;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 모션 블러 값만 저장하는 로직
bool UHorrorGameGameInstance::MotionBlurSaveLogic(bool inMotionBlur)
{
	// 모션 블러 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.bMotionBlur = inMotionBlur;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.bMotionBlur = OptionSetting.bMotionBlur;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 볼륨 값만 저장하는 로직.
bool UHorrorGameGameInstance::VolumeSaveLogic(float inVolume)
{
	// 볼륨 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.Volume = inVolume;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.Volume = OptionSetting.Volume;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 마우스 민감도 값만 저장하는 로직.
bool UHorrorGameGameInstance::MouseSensitiveSaveLogic(float inMouseSensitive)
{
	// 마우스 민감도 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.MouseSensitive = inMouseSensitive;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.MouseSensitive = OptionSetting.MouseSensitive;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 조준점 설정 값만 저장하는 로직.
bool UHorrorGameGameInstance::CrossHairOnSaveLogic(bool inIsCrossHairOn)
{
	// 조준점 설정 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.bIsCrossHairOn = inIsCrossHairOn;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.bIsCrossHairOn = OptionSetting.bIsCrossHairOn;
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 게임플레이 타이머 설정 값만 저장하는 로직.
bool UHorrorGameGameInstance::TimerOnSaveLogic(bool inIsTimerOn)
{
	// 게임플레이 타이머 설정 값을 가져와 세이브 데이터에 영구히 저장함.
	OptionSetting.bIsTimerOn = inIsTimerOn;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		SaveData->OptionSetting.bIsTimerOn = OptionSetting.bIsTimerOn;
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

// 챕터를 클리어할 때 클리어한 챕터의 데이터를 저장하는 로직.
bool UHorrorGameGameInstance::ChapterClearSaveLogic(int32 inClearedChapter, float inClearTime, int32 inNextChapter)
{
	//CurrentClearedTime = inClearTime;
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		// 갱신이 가능한지(처음 저장)
		bool bIsFirstSave = true; 
		
		float ClearTime = inClearTime;

		// 클리어한 챕터 번호를 찾을 때까지 반복함.
		for (int i = 0; i < SaveData->ClearChapter.Num(); ++i)
		{
			// 클리어한 챕터와 동일한 인덱스일 경우
			if (SaveData->ClearChapter[i].ChapterNumber == inClearedChapter)
			{
				// 이전 기록보다 더 짧은 기록을 냈다면 기록 경신
				if (SaveData->ClearChapter[i].NormalClearTime > ClearTime) 
				{
					SaveData->ClearChapter[i].NormalClearTime = ClearTime;
				}

				// 보통 난이도를 클리어한 상태가 아니라면 보통 난이도를 클리어했다고 설정함.
				if (!SaveData->ClearChapter[i].bIsNormalCleared)
				{
					ClearChapter[i].bIsNormalCleared = true;
					SaveData->ClearChapter[i].bIsNormalCleared = true;
				}

				// 후에 난이도별로 저장하는 로직도 필요함. 

				//if (SaveData->ClearChapter[i].bIsNormalCleared)
				//{
				//	/*TArray<FString>Times;
				//	float ParsedTime = 0.f;
				//	SaveData->ClearChapter[i].ClearTime.ParseIntoArray(Times, TEXT(":"), false);
				//	ParsedTime += FCString::Atof(*Times[0]) * 60 * 60;
				//	ParsedTime += FCString::Atof(*Times[1]) * 60;
				//	ParsedTime += FCString::Atof(*Times[2]);*/
				//	if (SaveData->ClearChapter[i].NormalClearTime > ClearTime) // 이전 기록보다 더 짧은 기록을 냈다면
				//	{
				//		SaveData->ClearChapter[i].NormalClearTime = ClearTime; // 기록 갱신
				//		//bCanUpdate = false;
				//	}
				//	//else // 짧은 기록이 아니라면
				//	//{
				//	//	bCanUpdate = false; // 갱신 안 되게 설정
				//	//}
				//	bIsFirstSave = false;
				//}
				//break;
			}

			// 다음 챕터의 상태를 열림 상태로 바꿈.
			if (SaveData->ClearChapter[i].ChapterNumber == inNextChapter)
			{
				if (!SaveData->ClearChapter[i].bIsOpened)
				{
					ClearChapter[i].bIsOpened = true;
					SaveData->ClearChapter[i].bIsOpened = true;
				}
				break;
			}
		}
		
		// 그 후 세이브 데이터에 영구히 저장함.
		SaveData->SaveData();
		return true;
	}
	return false;
}

// 요괴 특징 문서를 저장하는 로직.
bool UHorrorGameGameInstance::CharacteristicsOnSaveLogic()
{
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if(!SaveData->CollectArchives.SeeCharacteristic)
		{
			//SaveData->SeeCharacteristic = true;
			SaveData->CollectArchives.SeeCharacteristic = true;
			SaveData->SaveData();
			return true;
		}
	}
	return false;
}

// 도움말 문서를 저장하는 로직.
bool UHorrorGameGameInstance::TipsOnSaveLogic()
{
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if (!SaveData->CollectArchives.SeeHowToEscape)
		{
			//SaveData->SeeHowToEscape = true;
			SaveData->CollectArchives.SeeHowToEscape = true;
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
	return OptionSetting.BrightGamma;
}

bool UHorrorGameGameInstance::GetMotionBlur()
{
	return OptionSetting.bMotionBlur;
}

float UHorrorGameGameInstance::GetVolume()
{
	return OptionSetting.Volume;
}

float UHorrorGameGameInstance::GetMouseSensitive()
{
	return OptionSetting.MouseSensitive;
}

bool UHorrorGameGameInstance::GetIsCrossHairOn()
{
	return OptionSetting.bIsCrossHairOn;
}

bool UHorrorGameGameInstance::GetIsTimerOn()
{
	return OptionSetting.bIsTimerOn;
}

ELanguage UHorrorGameGameInstance::GetCurrentLanguage()
{
	return OptionSetting.Language;
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

TArray<FClearData> UHorrorGameGameInstance::GetAllClearData()
{
	return ClearChapter;
}