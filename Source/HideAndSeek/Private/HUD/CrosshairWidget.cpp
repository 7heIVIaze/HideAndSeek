// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/CrosshairWidget.h"
#include "Components/Image.h"

void UCrosshairWidget::SetCrossHairVisible(bool value)
{
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
