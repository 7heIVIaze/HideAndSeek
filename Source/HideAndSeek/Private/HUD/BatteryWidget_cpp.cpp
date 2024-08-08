// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/BatteryWidget_cpp.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Player/HorrorGameCharacter.h"

void UBatteryWidget_cpp::NativeConstruct()
{
	Super::NativeConstruct();
	/*Battery = Cast<UProgressBar>(GetWidgetFromName("BatteryUI"));
	BatteryImage = Cast<UImage>(GetWidgetFromName("BatteryImg"));*/
}

// 남은 배터리의 양을 그림으로 보여줄 함수임.
void UBatteryWidget_cpp::SetBatteryPercent(int32 value)
{
	float percent = (float)value / 200.0f;
	Battery->SetPercent(percent);
}

// 플레이어가 플래시 라이트를 선택했을 때 이 위젯을 띄울 함수임. 선택 안하면 안 띄움.
void UBatteryWidget_cpp::SetBatteryVisibility(bool value)
{
	if (value)
	{
		Battery->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Battery->SetVisibility(ESlateVisibility::Collapsed);
	}
}