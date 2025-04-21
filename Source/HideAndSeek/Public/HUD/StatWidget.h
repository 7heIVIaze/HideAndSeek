// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "StatWidget.generated.h"


/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UStatWidget : public UCustomUI
{
	GENERATED_BODY()

protected:
	// Constructor
	virtual void NativeConstruct() override;

	// Native Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	// HP Bar Update Function.
	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float NewHP);

	// Stamina Bar Update Function.
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaBar(int32 NewStaminaGuage);

	// Panic Bar Update Function.
	UFUNCTION(BlueprintCallable)
	void UpdatePanicBar(float NewPanicGuage);

	UFUNCTION(BlueprintCallable)
	void ShakeHeathBar();

public:
	// Initialize the stat component for delegate binding.
	UFUNCTION(BlueprintCallable)
	bool InitializeWidget(UPlayerStatComponent* StatComponent);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HPBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> HPText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PanicBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadWrite, Category = "Shake")
	bool bIsShaking;

	UPROPERTY(BlueprintReadWrite, Category = "Shake")
	float ShakeElapsedTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shake")
	float ShakeIntensity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shake")
	float ShakeDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Shake")
	FVector2D OriginalPosition;
};