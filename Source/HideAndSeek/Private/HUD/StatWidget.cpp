// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/StatWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Player/HorrorGameCharacter.h"
#include "Player/PlayerStatComponent.h"


void UStatWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UStatWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (bIsShaking)
	{
		ShakeElapsedTime += DeltaTime;

		if (ShakeElapsedTime < ShakeDuration)
		{
			float ShakeTime = ShakeElapsedTime * 30.0f;
			float OffsetX = FMath::Sin(ShakeTime) * ShakeIntensity;
			float OffsetY = FMath::Cos(ShakeTime) * ShakeIntensity;

			FVector2D NewPosition = OriginalPosition + FVector2D(OffsetX, OffsetY);
			HPBar->SetRenderTranslation(NewPosition);
		}
		else
		{
			bIsShaking = false;
			HPBar->SetRenderTranslation(OriginalPosition);
		}
	}
}

bool UStatWidget::InitializeWidget(UPlayerStatComponent* StatComponent)
{
	if (StatComponent)
	{
		// Bind the delegate.
		StatComponent->OnHealthPointChanged.AddDynamic(this, &UStatWidget::UpdateHPBar);

		UpdateHPBar(StatComponent->CurrentHP);

		return true;
	}

	return false;
}

void UStatWidget::UpdateHPBar(float NewHP)
{
	if (HPBar)
	{
		float BeforeHPPercent = HPBar->GetPercent();
		float HPPercent = NewHP / 200.0f; // Max HP is 200.0f
		UE_LOG(LogTemp, Warning, TEXT("Current HP Percent: %f"), HPPercent);
		HPBar->SetPercent(HPPercent);

		if (BeforeHPPercent > HPPercent)
		{
			ShakeHeathBar();
		}
	}

	if (HPText)
	{
		int HP = NewHP / 2.0f; // Max HP is 200.0f; Make HP have 0~100
		FString HPString = FString::Printf(TEXT("%d/100"), HP);

		HPText->SetText(FText::FromString(HPString));
	}
}

void UStatWidget::ShakeHeathBar()
{
	if (!bIsShaking)
	{
		ShakeElapsedTime = 0.0f;
		OriginalPosition = HPBar->GetRenderTransform().Translation;
		bIsShaking = true;
	}
}

//void UStatWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//	/*Battery = Cast<UProgressBar>(GetWidgetFromName("BatteryUI"));
//	BatteryImage = Cast<UImage>(GetWidgetFromName("BatteryImg"));*/
//}
//
//// 남은 배터리의 양을 그림으로 보여줄 함수임.
//void UStatWidget::SetBatteryPercent(int32 value)
//{
//	float percent = (float)value / 200.0f;
//	Battery->SetPercent(percent);
//}
//
//// 플레이어가 플래시 라이트를 선택했을 때 이 위젯을 띄울 함수임. 선택 안하면 안 띄움.
//void UStatWidget::SetBatteryVisibility(bool value)
//{
//	if (value)
//	{
//		Battery->SetVisibility(ESlateVisibility::Visible);
//	}
//	else
//	{
//		Battery->SetVisibility(ESlateVisibility::Collapsed);
//	}
//}