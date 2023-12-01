// CopyrightNotice=0 2023 Sunggon Kim

#include "StaminaWidget.h"
#include "Components/ProgressBar.h"
#include "HorrorGame/HorrorGameCharacter.h"

void UStaminaWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Stamina = Cast<UProgressBar>(GetWidgetFromName(TEXT("StaminaBar")));
	
}

void UStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Stamina->GetPercent() < 0.3f)
	{
		Stamina->SetFillColorAndOpacity(FLinearColor(1.0f, 0.2f, 0.0f));
	}
	else
	{
		Stamina->SetFillColorAndOpacity(FLinearColor(0.1f, 1.0f, 0.1f));
	}
}

void UStaminaWidget::SetStamina(int32 iValue)
{
	float fPercent = (float)iValue / 400.0f;
	Stamina->SetPercent(fPercent);
}

float UStaminaWidget::GetStamina()
{
	return Stamina->GetPercent();
}