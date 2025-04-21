// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/GameUI.h"
#include "HUD/InventoryWidget.h"
#include "HUD/InteractDot_cpp.h"
#include "HUD/StaminaWidget.h"
#include "HUD/StatWidget.h"
#include "HUD/MessageManagerWidget.h"
#include "HUD/ObjectWidget.h"
#include "HUD/ExplainWidget_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PatienceWidget.h"
#include "HUD/CrosshairWidget.h"
#include "HUD/TimerWidget.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/InventoryComponent.h"
#include "Player/PlayerStatComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"

// 인게임 UI 생성자.
void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	MainInterface->SetVisibility(ESlateVisibility::Collapsed);

	// 뷰포트에 출력이 될 때, 플레이어가 잡힌 경우
	if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(GetOwningPlayerPawn()))
	{
		UInventoryComponent* InventoryComp = PlayerCharacter->GetInventoryComponent();
		if (InventoryComp)
		{
			InitializeInventoryWidget(InventoryComp);
		}

		UPlayerStatComponent* PlayerStatComp = PlayerCharacter->GetStatComponent();
		if (PlayerStatComp)
		{
			InitializeStatWidget(PlayerStatComp);
			PlayerStatComp->OnDeath.AddDynamic(TimerWidget, &UTimerWidget::StopTimer);
		}
	}
}
//
void UGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
}

// 모든 위젯 초기화하는 함수.
void UGameUI::AllWidgetInit()
{
	// 모든 위젯을 초기화 해줌.
	MainInterface->SetVisibility(ESlateVisibility::HitTestInvisible);
	Init();
	SetInteractDotText(NSLOCTEXT("UGameUI", "None", ""));
	SetInteractDotErrorText(NSLOCTEXT("UGameUI", "None", ""));
	SetInteractDotExplainText(NSLOCTEXT("UGameUI", "None", ""));
	SetInteractDot(false);
	
	// 그와중에 프롤로그 챕터라면 오브젝트 위젯은 안 보이게 설정함.
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Contains(TEXT("Prologue")))
	{
		ObjectWidget->SetRenderOpacity(0.f);
	}
}

// 내부의 위젯들을 초기화하는 함수.
void UGameUI::Init()
{
	// 인벤토리 위젯이 존재하면, 플레이어를 할당하고, 해당 위젯을 초기화함,
	if (InventoryWidget)
	{
		InventoryWidget->Player = this->Player;
		InventoryWidget->Init();
	}

	if (ObjectWidget)
	{
		ObjectWidget->Init();
	}

	if (TimerWidget)
	{
		TimerWidget->Init();
	}
}

bool UGameUI::InitializeStatWidget(UPlayerStatComponent* StatComponent)
{
	if (StatWidget)
	{
		StatWidget->InitializeWidget(StatComponent);

		return true;
	}

	return false;
}

bool UGameUI::InitializeInventoryWidget(UInventoryComponent* InventoryComponent)
{
	if (InventoryWidget)
	{
		InventoryWidget->InitializeWidget(InventoryComponent);

		return true;
	}

	return false;
}

bool UGameUI::InitializeObjectWidget(UInventoryComponent* InventoryComponent)
{
	if (ObjectWidget)
	{
		ObjectWidget->InitializeWidget(InventoryComponent);

		return true;
	}

	return false;
}

// 조준점 위젯을 설정하는 함수.
void UGameUI::SetInteractDot(bool Value)
{
	if (InteractDotState == nullptr)
	{
		return;
	}

	// 상호작용 가능한 물체를 볼 때 생기는 조준점을 보이게 할 것인지 설정함.
	InteractDotState->SetInteractDot(Value);
	
	// 일반 조준점의 경우
	if (IsValid(Crosshair))
	{
		// 플레이어가 껐으면 안 보이게 설정함.
		if (Value)
		{
			Crosshair->SetVisibility(ESlateVisibility::Collapsed);
		}
		// 플레이어가 킨 경우 보이게 설정함.
		else
		{
			Crosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	
}

// 어떤 상호작용이 가능한지 설명을 위한 텍스트 문구를 설정할 함수.
void UGameUI::SetInteractDotText(FText strText)
{
	if (InteractDotState == nullptr)
	{
		return;
	}

	if (IsValid(InteractDotState)) // 여기 에러
	{
		InteractDotState->SetInteractText(strText);
	}
}

// 상호작용 실패 텍스트를 출력할 함수
void UGameUI::SetInteractDotErrorText(FText strText)
{
	if (InteractDotState == nullptr)
	{
		return;
	}

	// 
	if (IsValid(InteractDotState))
	{
		InteractDotState->SetErrorText(strText);
	}
}

// 튜토리얼을 위한 텍스트를 설정할 함수.
void UGameUI::SetInteractDotExplainText(FText strText)
{
	if (ExpWidget == nullptr)
	{
		return;
	}

	if (IsValid(ExpWidget))
	{
		ExpWidget->SetExplainText(strText);
	}
}

UInteractDot_cpp* UGameUI::GetInteractDotWidget() const
{
	return InteractDotState;
}

UTimerWidget* UGameUI::GetTimerWidget() const
{
	return TimerWidget;
}

// 게임 플레이 타이머 위젯이 보이게 할 것인지 설정할 함수.
void UGameUI::SetTimerWidget(bool value)
{
	if (IsValid(TimerWidget))
	{
		// 플레이어가 타이머를 켰으면 보이게 설정함.
		if (value)
		{
			TimerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		// 껐으면 안 보이게 설정함.
		else
		{
			TimerWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("There is no TimerWidget!")));
	}
}

// 게임 플레이 타이머를 멈추게 할 함수.
void UGameUI::SetTimerStop(bool inStop)
{
	if (IsValid(TimerWidget))
	{
		if (inStop)
		{
			TimerWidget->StopTimer();
		}
	}
}

void UGameUI::SetCrossHairVisible(bool value)
{
	//CrossVisible = value;

	Crosshair->SetCrossHairVisible(value);
}

// 문서가 문서 보관함에 저장될 때 텍스트를 출력할 함수.
void UGameUI::SetArchiveGetText(FText inText)
{
	if (ArchiveGetText == nullptr)
	{
		return;
	}

	ArchiveGetText->SetText(inText);
}

void UGameUI::ShowMessage(FText Message)
{
	if (MessageWidget)
	{
		MessageWidget->AddMessage(Message);
	}
}