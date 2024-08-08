// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/GameUI.h"
#include "HUD/Inventory_cpp.h"
#include "HUD/CutterWidget.h"
#include "HUD/InteractDot_cpp.h"
#include "HUD/StaminaWidget.h"
#include "HUD/BatteryWidget_cpp.h"
#include "HUD/CutterWidget.h"
#include "HUD/ExtinguisherWidget.h"
#include "HUD/ObjectWidget.h"
#include "HUD/ExplainWidget_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PatienceWidget.h"
#include "HUD/CrosshairWidget.h"
#include "HUD/TimerWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"

// 인게임 UI 생성자.
void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	//DiedWidget = Cast<UDiedWidget>(GetWidgetFromName(TEXT("DiedWidget_BP")));
	/*StaminaWidget = Cast<UStaminaWidget>(GetWidgetFromName(TEXT("Stamina_HUD_BP")));
	InteractDotState = Cast<UInteractDot_cpp>(GetWidgetFromName(TEXT("UI_InteractDot")));
	BatteryWidget = Cast<UBatteryWidget_cpp>(GetWidgetFromName(TEXT("BatteryWidget_BP")));
	CutterWidget = Cast<UCutterWidget>(GetWidgetFromName("Cutter_HUD_BP"));
	ExtWidget = Cast<UExtinguisherWidget>(GetWidgetFromName("Extinguisher_HUD_BP"));
	ObjWidget = Cast<UObjectWidget>(GetWidgetFromName("Object_UI_BP"));
	ExpWidget = Cast< UExplainWidget_cpp>(GetWidgetFromName("BP_ExplainWidget"));
	PatienceWidget = Cast<UPatienceWidget>(GetWidgetFromName("Patience_UI_BP"));*/

	MainInterface->SetVisibility(ESlateVisibility::Collapsed);
}
//
void UGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 현재 스테미너의 잔량이 100%일 경우, 스태미너 위젯을 안보이게 함.
	if (StaminaWidget->GetStamina() >= 1.0f)
	{
		StaminaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	// 100%가 아닐 경우, 위젯을 보이게 설정함.
	else
	{
		StaminaWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	// 플레이어가 플래시 라이트를 선택한 상태라면, 배터리 위젯을 보이게 함.
	if (BatteryVisible)
	{
		BatteryWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	// 아니라면 안 보이게 함.
	else
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 플레이어가 절단기를 선택한 상태라면, 절단기 위젯을 보이게 함.
	if (CutterVisible)
	{
		CutterWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	// 아니라면 안 보이게 함.
	else
	{
		CutterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 플레이어가 소화기를 선택한 상태라면, 소화기 위젯을 보이게 함.
	if (ExtVisible)
	{
		ExtWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	// 아니라면 안 보이게 함.
	else
	{
		ExtWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

// 모든 위젯 초기화하는 함수.
void UGameUI::AllWidgetInit()
{
	// 모든 위젯을 초기화 해줌.
	MainInterface->SetVisibility(ESlateVisibility::HitTestInvisible);
	Init();
	SetInteractDotText(NSLOCTEXT("UGameUI", "InteractDotText", ""));
	SetInteractDotErrorText(NSLOCTEXT("UGameUI", "InteractDotErrorText", ""));
	SetInteractDotExplainText(NSLOCTEXT("UGameUI", "InteractDotExplainText", ""));
	SetInteractDot(false);
	//GameUIWidget->SetBaseInterface(true);
	SetStaminaHUD(400);
	SetBatteryWidget(false);
	SetCutterHUD(5);
	SetCutterWidget(false);
	SetExtHUD(100);
	SetExtWidget(false);
	SetObjectCount(1, 0);
	SetObjectCount(2, 0);
	SetObjectCount(3, 0);
	SetPatience(0);
	TimerWidget->Init();

	// 그와중에 프롤로그 챕터라면 오브젝트 위젯은 안 보이게 설정함.
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Contains(TEXT("Prologue")))
	{
		ObjWidget->SetRenderOpacity(0.f);
	}
}

// 인벤토리를 초기화하는 함수.
void UGameUI::Init()
{
	// 인벤토리 위젯이 존재하면, 플레이어를 할당하고, 해당 위젯을 초기화함,
	if (Inventory)
	{
		Inventory->Player = this->Player;
		Inventory->Init();
	}
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

// 스테미너 위젯의 스테미너 양을 설정할 함수.
void UGameUI::SetStaminaHUD(int32 iValue)
{
	if (IsValid(StaminaWidget))
	{
		StaminaWidget->SetStamina(iValue);
	}
}

// 배터리 위젯의 배터리 잔량을 설정할 함수.
void UGameUI::SetBatteryHUD(int32 iValue)
{
	if (IsValid(BatteryWidget))
	{
		BatteryWidget->SetBatteryPercent(iValue);
	}
}

void UGameUI::SetBatteryWidget(bool value)
{
	BatteryVisible = value;
}

// 절단기 위젯의 절단기 내구도를 설정할 함수.
void UGameUI::SetCutterHUD(int32 iValue)
{
	if (IsValid(CutterWidget))
	{
		CutterWidget->SetCutterPercent(iValue);
	}
}

void UGameUI::SetCutterWidget(bool value)
{
	CutterVisible = value;
}

// 소화기 위젯의 분말 잔량을 설정할 함수.
void UGameUI::SetExtHUD(int32 value)
{
	if (IsValid(ExtWidget))
	{
		ExtWidget->SetExtinguisherPercent(value);
	}
}

void UGameUI::SetExtWidget(bool value)
{
	ExtVisible = value;
}

// 오브젝트 위젯의 수집한 오브젝트의 양을 설정할 함수.
void UGameUI::SetObjectCount(unsigned int idx, int32 value)
{
	if (IsValid(ObjWidget))
	{
		ObjWidget->SetObjectCount(idx, value);
	}
}

// 패닉게이지 위젯의 착란의 양을 설정할 함수.
void UGameUI::SetPatience(int32 value)
{
	if (IsValid(PatienceWidget))
	{
		PatienceWidget->SetPatience(value);
	}
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