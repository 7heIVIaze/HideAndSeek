// Fill out your copyright notice in the Description page of Project Settings.


#include "HorrorGamePlayerController.h"
#include "GameUI.h"
#include "DiedWidget.h"
#include "PauseWidget.h"
#include "Kismet/GameplayStatics.h"

AHorrorGamePlayerController::AHorrorGamePlayerController()
{
	static ConstructorHelpers::FClassFinder<UGameUI>MainHUD(TEXT("/Game/Assets/BluePrints/UI/BasicInterface"));
	if (MainHUD.Succeeded())
	{
		MainWidgetClass = MainHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UDiedWidget>DiedHUD(TEXT("/Game/Assets/BluePrints/UI/DiedWidget_BP"));
	if (DiedHUD.Succeeded())
	{
		DeadWidgetClass = DiedHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UPauseWidget>PauseHUD(TEXT("/Game/Assets/BluePrints/UI/PauseWidget_BP"));
	if (PauseHUD.Succeeded())
	{
		PauseWidgetClass = PauseHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>LoadingHUD(TEXT("/Game/Assets/BluePrints/UI/BP_LoadingWidget"));
	if (LoadingHUD.Succeeded())
	{
		LoadingWidgetClass = LoadingHUD.Class;
	}

	GameInputMode = FInputModeGameOnly();
	UIInputMode = FInputModeUIOnly();
}

void AHorrorGamePlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHorrorGamePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AHorrorGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(nullptr == MainWidget)
		MainWidget = CreateWidget<UGameUI>(this, MainWidgetClass);
	/*if (IsValid(MainWidget))
	{
		MainWidget->AddToViewport(1);
		ChangeInputMode(true);
	}*/

	if(nullptr == DeadWidget)
		DeadWidget = CreateWidget<UDiedWidget>(this, DeadWidgetClass);

	if (nullptr == LoadingWidget)
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);

	FString strText = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (strText == TEXT("Level1"))
	{
		if (IsValid(LoadingWidget))
		{
			LoadingWidget->AddToViewport();
			ChangeInputMode(false);
		}
	}
	else
	{
		if (IsValid(MainWidget))
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("MainWidget is Here")));
			MainWidget->AddToViewport();
			ChangeInputMode(true);
		}
	}
}

void AHorrorGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AHorrorGamePlayerController::OnGamePause);
}

void AHorrorGamePlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AHorrorGamePlayerController::ShowDeadUI()
{
	MainWidget->RemoveFromParent();
	DeadWidget->AddToViewport();
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(DeadWidget->TakeWidget());
	ChangeInputMode(false);
	/*if (nullptr != MainWidget)
	{
		MainWidget = nullptr;
	}*/
}

void AHorrorGamePlayerController::ShowMainUI()
{
	if (IsValid(MainWidget))
	{
		if(IsValid(LoadingWidget))
			if(LoadingWidget->IsInViewport())
				LoadingWidget->RemoveFromParent();
		MainWidget->AddToViewport(1);
		ChangeInputMode(true);
	}
}

class UGameUI* AHorrorGamePlayerController::GetMainWidget() const
{
	return MainWidget;
}

void AHorrorGamePlayerController::OnGamePause()
{
	PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
	PauseWidget->AddToViewport();
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
	ChangeInputMode(false);
	SetPause(true);
}