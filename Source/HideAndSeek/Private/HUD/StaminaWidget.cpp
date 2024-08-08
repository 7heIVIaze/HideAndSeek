// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/StaminaWidget.h"
#include "Components/ProgressBar.h"
#include "Player/HorrorGameCharacter.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
//	Stamina = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
}

void UStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 스테미너의 잔량이 30% 미만이면 주황색으로
	if (StaminaBar->GetPercent() < 0.3f)
	{
		StaminaBar->SetFillColorAndOpacity(FLinearColor(1.0f, 0.2f, 0.0f));
	}
	// 아니면 초록색으로 보이게 설정함.
	else
	{
		StaminaBar->SetFillColorAndOpacity(FLinearColor(0.1f, 1.0f, 0.1f));
	}
}

// 스테미너 잔량을 설정하는 함수.
void UStaminaWidget::SetStamina(int32 iValue)
{
	float fPercent = (float)iValue / 400.0f;
	StaminaBar->SetPercent(fPercent);
}

float UStaminaWidget::GetStamina()
{
	return StaminaBar->GetPercent();
}