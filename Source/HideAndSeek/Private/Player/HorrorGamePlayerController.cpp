// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Player/HorrorGamePlayerController.h"
#include "HUD/GameUI.h"
#include "HUD/DiedWidget.h"
#include "HUD/PauseWidget.h"
#include "HUD/ClearWidget.h"
#include "HUD/DocumentWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HorrorGameCharacter.h"

// 컨트롤러 생성자.
AHorrorGamePlayerController::AHorrorGamePlayerController()
{
	// 위젯 블루프린트를 부모 클래스로 호출해 할당함.
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

	// Input 모드를 관리하기 쉽게 변수로 설정함.
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

	// 생성자에서 받은 위젯 클래스를 기반으로 위젯을 생성함.
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

	// 현재 챕터(레벨)의 이름을 가져옴.
	FString strText = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// 챕터 명이 프롤로그(챕터 1: 지하실)일 경우
	if (strText.Equals(TEXT("Prologue")) || strText.Contains(TEXT("Test")))
	{
		if (IsValid(MainWidget))
		{
			// 메인 UI 위젯을 그대로 뷰포트에 출력하고, Input 모드를 인게임 전용으로 변경함.
			MainWidget->AddToViewport();
			ChangeInputMode(true);

			// 또한 플레이어의 메인 UI를 출력한 UI로 설정해주고, 레벨이 시작했다고 알림.
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
	// 그 외 챕터일 경우
	else if(strText.Contains(TEXT("Level")))
	{
		// 로딩 화면을 뷰포트에 출력하고, Input 모드를 UI 전용으로 변경함.
		if (IsValid(LoadingWidget))
		{
			LoadingWidget->AddToViewport();
			ChangeInputMode(false);
		}

		// 플레이어의 메인 UI를 Main Widget으로 설정함.
		if (IsValid(MainWidget))
		{
			MainWidget->AddToViewport(); // <- 이거 없으면 터짐. 밑의 할당해주고 난 뒤의 문제로 인함.
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
}

void AHorrorGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AHorrorGamePlayerController::OnGamePause);
}

// Input Mode를 바꿔줌.
void AHorrorGamePlayerController::ChangeInputMode(bool bGameMode)
{
	// true면 인게임 Input으로
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	// false면 UI Input으로
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

// 사망 UI를 보여주는 함수.
void AHorrorGamePlayerController::ShowDeadUI()
{
	// 메인 UI를 뷰포트에서 제거함.
	MainWidget->RemoveFromParent();

	// 사망 UI를 뷰포트에 출력하고, UI Input으로 설정함.
	DeadWidget->AddToViewport();
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(DeadWidget->TakeWidget());
	ChangeInputMode(false);
}

// 사망 시 요괴 문서 획득 문구 출력을 해주는 함수.
void AHorrorGamePlayerController::SetDeadUIText(FText inText)
{
	DeadWidget->SetArchiveGetText(inText);
}

// 메인 UI를 보여주는 함수.
void AHorrorGamePlayerController::ShowMainUI()
{
	if (IsValid(MainWidget))
	{
		// 로딩 화면이 뷰포트에 존재하면 제거함.
		if (IsValid(LoadingWidget))
		{
			if (LoadingWidget->IsInViewport())
			{
				LoadingWidget->RemoveFromParent();
			}
		}
		
		// 인게임 Input으로 변경하고, 플레이어에게 레벨이 시작되었다고 알림.
		ChangeInputMode(true);

		if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetPawn()))
		{
			PlayerCharacter->LevelStart();
		}
	}
}

class UGameUI* AHorrorGamePlayerController::GetMainWidget() const
{
	return MainWidget;
}

// 일시 정지를 시킬 함수.
void AHorrorGamePlayerController::OnGamePause()
{
	// 일시 정지 위젯을 생성해 뷰포트에 출력함.
	PauseWidget = CreateWidget<UPauseWidget>(this, PauseWidgetClass);
	PauseWidget->AddToViewport();

	// Input Mode를 UI 전용으로 바꾸고, 게임을 일시 정지함.
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(PauseWidget->TakeWidget());
	ChangeInputMode(false);
	SetPause(true);
}

// 클리어 UI를 출력할 함수.
void AHorrorGamePlayerController::OnGameClear(FString inClearedTime, bool bIsRecordBreaking)
{
	// 메인 UI를 뷰포트에서 제거하고, Clear UI를 뷰포트에 출력함.
	MainWidget->RemoveFromParent();
	ClearWidget->AddToViewport();
	ClearWidget->SetClearTime(inClearedTime);
	ClearWidget->SetRecordBreakingText(bIsRecordBreaking);
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(ClearWidget->TakeWidget());
	ChangeInputMode(false);
}

// 문서를 확인할 때 UI를 출력할 함수.
void AHorrorGamePlayerController::OnDocumentWidget(FText inText, FString inType)
{
	// 문서 위젯를 생성하고 뷰포트에 출력함.
	DocumentWidget = CreateWidget<UDocumentWidget>(this, DocumentWidgetClass);
	DocumentWidget->AddToViewport();
	
	// 문서가 기사류이면, 문서 위젯을 기사 모드로 설정하고 텍스트를 지정해줌.
	if (inType.Equals("Article"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Article);
		DocumentWidget->SetArticleText(inText);
	}
	// 문서가 일기류이면, 문서 위젯을 일기 모드로 설정하고 텍스트를 지정해줌.
	else if (inType.Equals("Diary"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Letter);
		DocumentWidget->SetDocumentText(inText);
	}
	// 문서가 편지류이면, 문서 위젯을 편지 모드로 설정하고 텍스트를 지정해줌.
	else if (inType.Equals("Letter"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Letter);
		DocumentWidget->SetDocumentText(inText);
	}
	// 문서가 도움말류, 문서 위젯을 도움말 모드로 설정하고 텍스트를 지정해줌.
	else if (inType.Equals("Hint"))
	{
		DocumentWidget->SetCurrentDocumentMode(EDocument::D_Hint);
		DocumentWidget->SetTipsText(inText);
	}

	// Input을 UI 전용으로  설정하고, 게임을 일시 정지함.
	UIInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	UIInputMode.SetWidgetToFocus(DocumentWidget->TakeWidget());
	ChangeInputMode(false);
	SetPause(true);
}

// 엔딩을 보여주는 함수. (임시(1.X.X 버전)용으로 미래엔 삭제 예정)
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