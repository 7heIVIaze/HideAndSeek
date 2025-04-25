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
		StatComponent->OnStaminaChanged.AddDynamic(this, &UStatWidget::UpdateStaminaBar);
		StatComponent->OnPanicChanged.AddDynamic(this, &UStatWidget::UpdatePanicBar);

		UpdateHPBar(StatComponent->CurrentHP);
		UpdateStaminaBar(StatComponent->Stamina);
		UpdatePanicBar(StatComponent->ConfusionPoint);

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

void UStatWidget::UpdateStaminaBar(int32 NewStaminaGuage)
{
	if (StaminaBar)
	{
		float StaminaPercent = (float)NewStaminaGuage / 400.0f; // Max Stamina is 400.0f
		StaminaBar->SetPercent(StaminaPercent);

		if (StaminaPercent == 1.0f)
		{
			StaminaBar->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			StaminaBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UStatWidget::UpdatePanicBar(float NewPanicGuage)
{
	if (PanicBar)
	{
		float PanicPercent = NewPanicGuage / 100.0f; // Max PanicGuage is 100.0f
		PanicBar->SetPercent(PanicPercent);
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