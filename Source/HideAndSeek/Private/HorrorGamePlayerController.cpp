// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HorrorGamePlayerController.h"
#include "GameUI.h"
#include "DiedWidget.h"
#include "PauseWidget.h"
#include "HUD/ClearWidget.h"
#include "HUD/DocumentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HideAndSeek/HorrorGameCharacter.h"

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
	
	static ConstructorHelpers::FClassFinder<UUserWidget>ClearHUD(TEXT("/Game/Assets/BluePrints/UI/BP_ClearWidget"));
	if (ClearHUD.Succeeded())
	{
		ClearWidgetClass = ClearHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UDocumentWidget>DocumentHUD(TEXT("/Game/Assets/BluePrints/UI/BP_DocumentWidget"));
	if (DocumentHUD.Succeeded())
	{
		DocumentWidgetClass = DocumentHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>BadEndHUD(TEXT("/Game/Assets/BluePrints/UI/BP_Ending1"));
	if (BadEndHUD.Succeeded())
	{
		BadEndingWidgetClass = BadEndHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>NormalEndHUD(TEXT("/Game/Assets/BluePrints/UI/BP_Ending2"));
	if (NormalEndHUD.Succeeded())
	{
		NormalEndingWidgetClass = NormalEndHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>TrueEndHUD(TEXT("/Game/Assets/BluePrints/UI/BP_Ending3"));
	if (TrueEndHUD.Succeeded())
	{
		TrueEndingWidgetClass = TrueEndHUD.Class;
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

	if (nullptr == MainWidget)
	{
		MainWidget = CreateWidget<UGameUI>(this, MainWidgetClass);
	}

	if (nullptr == DeadWidget)
	{
		DeadWidget = CreateWidget<UDiedWidget>(this, DeadWidgetClass);
	}

	if (nullptr == LoadingWidget)
	{
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
	}

	if (nullptr == ClearWidget)
	{
		ClearWidget = CreateWidget<UClearWidget>(this, ClearWidgetClass);
	}


	FString strText = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (strText.Equals(TEXT("Level1")))
	{
		if (IsValid(LoadingWidget))
		{
			LoadingWidget->AddToViewport();
			ChangeInputMode(false);
		}

		if (IsValid(MainWidget))
		{
			MainWidget->AddToViewport();
			//ChangeInputMode(true);
			if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetPawn()))
			{
				if (PlayerCharacter->PlayerStatus == Player_Status::Hiding)
				{
					return;
				}
				PlayerCharacter->GameUIWidget = MainWidget;
			}
		}
	}
	else
	{
		if (IsValid(MainWidget))
		{
			MainWidget->AddToViewport();
			ChangeInputMode(true);

			if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetPawn()))
			{
				if (PlayerCharacter->PlayerStatus == Player_Status::Hiding)
				{
					return;
				}
				PlayerCharacter->GameUIWidget = MainWidget;
				PlayerCharacter->LevelStart();
			}
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
}

void AHorrorGamePlayerController::SetDeadUIText(FText inText)
{
	DeadWidget->SetArchiveGetText(inText);
	/*FTimerHandle ArchiveTextTimer;

	GetWorld()->GetTimerManager().SetTimer(ArchiveTextTimer, FTimerDelegate::CreateLambda([&]() {
		DeadWidget->SetArchiveGetText(FText::FromString(TEXT("")));
		GetWorld()->GetTimerManager().ClearTimer(ArchiveTextTimer);
	}), 10.f, false);*/
}

void AHorrorGamePlayerController::ShowMainUI()
{
	if (IsValid(MainWidget))
	{
		if (IsValid(LoadingWidget))
		{
			if (LoadingWidget->IsInViewport())
			{
				LoadingWidget->RemoveFromParent();
			}
		}
		MainWidget->AddToViewport();
		ChangeInputMode(true);

		if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetPawn()))
		{
			// PlayerCharacter->GameUIWidget = MainWidget;
			PlayerCharacter->LevelStart();
		}
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

void AHorrorGamePlayerController::OnGameClear(FString inClearedTime, bool bIsRecordBreaking)
{
	MainWidget->RemoveFromParent();
	ClearWidget->AddToViewport();
	ClearWidget->SetClearTime(inClearedTime);
	ClearWidget->SetRecordBreakingText(bIsRecordBreaking);
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(ClearWidget->TakeWidget());
	ChangeInputMode(false);
}

void AHorrorGamePlayerController::OnDocumentWidget(FText inText, FString inType)
{
	DocumentWidget = CreateWidget<UDocumentWidget>(this, DocumentWidgetClass);
	DocumentWidget->AddToViewport();
	if (inType.Equals("Article"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Article);
		DocumentWidget->SetArticleText(inText);
	}
	else if (inType.Equals("Diary"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Letter);
		DocumentWidget->SetDocumentText(inText);
	}
	else if (inType.Equals("Letter"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Letter);
		DocumentWidget->SetDocumentText(inText);
	}
	else if (inType.Equals("Hint"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Hint);
		DocumentWidget->SetTipsText(inText);
	}

	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(DocumentWidget->TakeWidget());
	ChangeInputMode(false);
	SetPause(true);
}

void AHorrorGamePlayerController::ShowEnding(int EndType)
{
	if (EndType == 0) // Bad Ending
	{
		EndingWidget = CreateWidget<UUserWidget>(this, BadEndingWidgetClass);
		EndingWidget->AddToViewport();
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UIInputMode.SetWidgetToFocus(EndingWidget->TakeWidget());
		ChangeInputMode(false);
	}
	else if (EndType == 1) // Normal Ending
	{
		EndingWidget = CreateWidget<UUserWidget>(this, NormalEndingWidgetClass);
		EndingWidget->AddToViewport();
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UIInputMode.SetWidgetToFocus(EndingWidget->TakeWidget());
		ChangeInputMode(false);
	}
	else if (EndType == 2) // True Ending
	{
		EndingWidget = CreateWidget<UUserWidget>(this, TrueEndingWidgetClass);
		EndingWidget->AddToViewport();
		UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		UIInputMode.SetWidgetToFocus(EndingWidget->TakeWidget());
		ChangeInputMode(false);
	}
}