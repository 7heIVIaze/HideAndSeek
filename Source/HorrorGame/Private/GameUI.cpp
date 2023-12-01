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
	if (nullptr == ObjWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ObjWidget NULL pointer"));
	}
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
		StaminaWidget->SetVisibility(ESlateVisibility::Visible);
	}

	if (BatteryVisible)
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (CutterVisible)
	{
		CutterWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CutterWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ExtVisible)
	{
		ExtWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ExtWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	/*if (ObjVisible)
	{
		ObjWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ObjWidget->SetVisibility(ESlateVisibility::Collapsed);
	}*/
}

void UGameUI::AllWidgetInit()
{
	Init();
	SetInteractDotText(TEXT(""));
	SetInteractDotErrorText(TEXT(""));
	SetInteractDotExplainText(TEXT(""));
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
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Prologue"))
	{
		ObjWidget->SetRenderOpacity(0.f);
	}
	

	//InteractDot = GameUIWidget->GetInteractDotWidget();
	/*if (UseInteractText)
	{
		UseInteractText->Player = this->Player;
		UseInteractText->InteractUIInit();
	}*/
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
	/*if (IsValid(InteractDotState))
	{
		if (Value)
		{
			InteractDotState->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InteractDotState->SetVisibility(ESlateVisibility::Collapsed);
		}
		InteractDotState->SetInteractDot(Value);
	}*/
	InteractDotState->SetInteractDot(Value);
}

void UGameUI::SetInteractDotText(FString strText)
{
	if (InteractDotState == nullptr) return;
	if (IsValid(InteractDotState)) // 여기 에러
	{
		InteractDotState->SetInteractText(strText);
	}
}

void UGameUI::SetInteractDotErrorText(FString strText)
{
	if (InteractDotState == nullptr) return;
	if (IsValid(InteractDotState))
	{
		InteractDotState->SetErrorText(strText);
	}
}

void UGameUI::SetInteractDotExplainText(FString strText)
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

//void UGameUI::SetBaseInterface(bool Value)
//{
//	if (Value) // Player Died
//	{
//		DiedWidget->SetVisibility(ESlateVisibility::Collapsed);
//
//	}
//	else
//	{
//		DiedWidget->SetVisibility(ESlateVisibility::Visible);
//	}
//}

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
	//BatteryWidget->SetBatteryVisibility(value);
	/*if (value)
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BatteryWidget->SetVisibility(ESlateVisibility::Collapsed);
	} */
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