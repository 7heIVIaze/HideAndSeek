// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/PatienceWidget.h"
#include "Components/ProgressBar.h"

// 패닉 게이지 위젯 생성자.
void UPatienceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//Mental = Cast<UProgressBar>(GetWidgetFromName(TEXT("PatienceBar")));
}

// 패닉 게이지 설정 함수.
void UPatienceWidget::SetPatience(int32 value)
{
	// Progress Bar의 범위는 0..1이기 때문에 100으로 나눠줌.
	float fPercent = (float)value / 100.f;
	PanicBar->SetPercent(fPercent);
}

float UPatienceWidget::GetPatience()
{
	return PanicBar->GetPercent();
}