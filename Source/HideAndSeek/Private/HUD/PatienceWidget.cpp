// CopyrightNotice=0 2023 Sunggon Kim

#include "HUD/PatienceWidget.h"
#include "Components/ProgressBar.h"

void UPatienceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Mental = Cast<UProgressBar>(GetWidgetFromName(TEXT("PatienceBar")));
}

void UPatienceWidget::SetPatience(int32 value)
{
	float fPercent = (float)value / 100.f;
	Mental->SetPercent(fPercent);
}

float UPatienceWidget::GetPatience()
{
	return Mental->GetPercent();
}