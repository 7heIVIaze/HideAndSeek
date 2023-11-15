// Fill out your copyright notice in the Description page of Project Settings.


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


UHorrorGameGameInstance::UHorrorGameGameInstance()
{
	//FString ItemDataPath = TEXT("/Script/Engine.DataTable'/Game/Assets/Items/Item.Item'");
	/*static ConstructorHelpers::FObjectFinder<UDataTable> DT_Item(*ItemDataPath);
	if (DT_Item.Succeeded())
	{
		HorrorGameTable = DT_Item.Object;
	}*/
	/*static ConstructorHelpers::FClassFinder<UStageSelectWidget>StageSelectWidgetClass(TEXT("/Game/Assets/BluePrints/UI/BP_StageSelectWidget_C"));
	if (StageSelectWidgetClass.Succeeded())
	{
		WidgetClass = StageSelectWidgetClass.Class;
	}*/

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
}

void UHorrorGameGameInstance::OnStart()
{
	Super::OnStart();
	/*BGM_Comp = UGameplayStatics::CreateSound2D(GetWorld(), OpeningCue, 1.f, 1.f, 0.f, nullptr, true, true);
	BGM_Comp->Play();*/
	//UGameplayStatics::SpawnSound2D(GetWorld(), OpeningCue, 1.f, 1.f, 0.f, nullptr, true, true);
}

FHorrorGameItemData* UHorrorGameGameInstance::GetHorrorGameItemData(int32 Name)
{
	return HorrorGameTable->FindRow<FHorrorGameItemData>(*FString::FromInt(Name), TEXT(""));
}

//void UHorrorGameGameInstance::ShowStageSelectWidget(bool bIsShow)
//{
//	if(bIsShow)
//	{
//		BGM_Comp->Stop();
//	}
//}

void UHorrorGameGameInstance::StopSound()
{
	if(BGM_Comp)
	BGM_Comp->Stop();
}

//void UHorrorGameGameInstance::ChangeWidget(TSubclassOf<UUserWidget> newWidget)
//{
//	if (CurrentWidget != nullptr)
//	{
//		CurrentWidget->RemoveFromParent();
//		CurrentWidget = nullptr;
//	}
//
//	if (newWidget != nullptr)
//	{
//		CurrentWidget = CreateWidget(GetWorld(), newWidget);
//		if (CurrentWidget != nullptr)
//		{
//			CurrentWidget->AddToViewport();
//		}
//	}
//}

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

		//else if (MapName.Contains(TEXT("Level1")))
		//{
		//	FLoadingScreenAttributes LoadingScreen;
		//	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		//	LoadingScreen.bMoviesAreSkippable = false;
		//	LoadingScreen.bWaitForManualStop = true;
		//	// LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		//	LoadingWidget = CreateWidget(this, WidgetClass);
		//	LoadingScreen.WidgetLoadingScreen = LoadingWidget->TakeWidget();

		//	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
		//}
	}
}

void UHorrorGameGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}