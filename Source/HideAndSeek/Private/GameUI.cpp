// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "GameUI.h"
#include "Inventory_cpp.h"
#include "CutterWidget.h"
#include "InteractDot_cpp.h"
#include "StaminaWidget.h"
#include "BatteryWidget_cpp.h"
#include "CutterWidget.h"
#include "ExtinguisherWidget.h"
#include "ObjectWidget.h"
#include "ExplainWidget_cpp.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/PatienceWidget.h"
#include "HUD/CrosshairWidget.h"
#include "HUD/TimerWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"

//UGameUI::UGameUI(const FObjectInitializer& ObjectInitializer)
//{
//
//
//}
void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	//DiedWidget = Cast<UDiedWidget>(GetWidgetFromName(TEXT("DiedWidget_BP")));
	StaminaWidget = Cast<UStaminaWidget>(GetWidgetFromName(TEXT("Stamina_HUD_BP")));
	InteractDotState = Cast<UInteractDot_cpp>(GetWidgetFromName(TEXT("UI_InteractDot")));
	BatteryWidget = Cast<UBatteryWidget_cpp>(GetWidgetFromName(TEXT("BatteryWidget_BP")));
	CutterWidget = Cast<UCutterWidget>(GetWidgetFromName("Cutter_HUD_BP"));
	ExtWidget = Cast<UExtinguisherWidget>(GetWidgetFromName("Extinguisher_HUD_BP"));
	ObjWidget = Cast<UObjectWidget>(GetWidgetFromName("Object_UI_BP"));
	ExpWidget = Cast< UExplainWidget_cpp>(GetWidgetFromName("BP_ExplainWidget"));
	PatienceWidget = Cast<UPatienceWidget>(GetWidgetFromName("Patience_UI_BP"));

	MainInterface->SetVisibility(ESlateVisibility::Collapsed);
	//TimerWidget = Cast<UTimerWidget>(GetWidgetFromName("BP_TimerWidget"));
	//DotCrossHair = Cast<UImage>(GetWidgetFromName("Crosshair"));
}
//
void UGameUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (StaminaWidget->GetStamina() >= 1.0f)
	{
		StaminaWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		StaminaWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	if (BatteryVisible)
	{
		BatteryWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CutterVisible)
	{
		CutterWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		CutterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ExtVisible)
	{
		ExtWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		ExtWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	//if (CrossVisible) // 크로스 헤어 설정을 on 했을 경우
	//{
	//	if (InteractVisible) // 어떤 사물과 인터랙트 가능한 경우
	//	{
	//		Crosshair->SetVisibility(ESlateVisibility::Collapsed);
	//	}
	//	else
	//	{
	//		Crosshair->SetVisibility(ESlateVisibility::Visible);
	//	}
	//}
	//else // 크로스 헤어 설정을 off 한 경우
	//{
	//	Crosshair->SetVisibility(ESlateVisibility::Collapsed);
	//}
	//Crosshair->SetVisibility(ESlateVisibility::Collapsed);
}

void UGameUI::AllWidgetInit()
{
	//LoadingWidget->SetVisibility(ESlateVisibility::Collapsed);
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
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()).Contains(TEXT("Prologue")))
	{
		ObjWidget->SetRenderOpacity(0.f);
	}
}

void UGameUI::Init()
{
	if (Inventory)
	{
		Inventory->Player = this->Player;
		Inventory->Init();
	}
	/*if (BatteryWidget)
	{
		BatteryWidget->Player = this->Player;
		BatteryWidget->Init();
	}*/
}

void UGameUI::SetInteractDot(bool Value)
{
	if (InteractDotState == nullptr) return;

	InteractDotState->SetInteractDot(Value);
	//InteractVisible = Value;
	if (IsValid(Crosshair))
	{
		if (Value)
		{
			Crosshair->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			Crosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	
}

void UGameUI::SetInteractDotText(FText strText)
{
	if (InteractDotState == nullptr) return;
	if (IsValid(InteractDotState)) // 여기 에러
	{
		InteractDotState->SetInteractText(strText);
	}
}

void UGameUI::SetInteractDotErrorText(FText strText)
{
	if (InteractDotState == nullptr) return;
	if (IsValid(InteractDotState))
	{
		InteractDotState->SetErrorText(strText);
	}
}

void UGameUI::SetInteractDotExplainText(FText strText)
{
	if (ExpWidget == nullptr) return;
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

void UGameUI::SetStaminaHUD(int32 iValue)
{
	if (IsValid(StaminaWidget))
	{
		StaminaWidget->SetStamina(iValue);
	}
}

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

void UGameUI::SetObjectCount(unsigned int idx, int32 value)
{
	if (IsValid(ObjWidget))
	{
		ObjWidget->SetObjectCount(idx, value);
	}
}

void UGameUI::SetPatience(int32 value)
{
	if (IsValid(PatienceWidget))
	{
		PatienceWidget->SetPatience(value);
	}
}

void UGameUI::SetTimerWidget(bool value)
{
	if (IsValid(TimerWidget))
	{
		if (value)
		{
			TimerWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
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

void UGameUI::SetArchiveGetText(FText inText)
{
	if (ArchiveGetText == nullptr) return;
	ArchiveGetText->SetText(inText);
}