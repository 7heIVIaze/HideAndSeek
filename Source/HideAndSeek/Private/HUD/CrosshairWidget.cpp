// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/CrosshairWidget.h"
#include "Components/Image.h"

// 조준점을 보이게 할지 안 보이게 할 지 설정하는 함수.
void UCrosshairWidget::SetCrossHairVisible(bool value)
{
	// 옵션에서 조준점 설정을 끄면 안 보이게, 켜면 보이게 설정함.
	DotVisible = value;
	if (DotVisible)
	{
		Crosshair->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		Crosshair->SetVisibility(ESlateVisibility::Collapsed);
	}
}
