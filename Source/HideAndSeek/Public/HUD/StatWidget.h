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

//protected:
//	virtual void NativeConstruct() override;
//
//public:
////	UUISlot_cpp(const FObjectInitializer& ObjectInitializer);
//	UFUNCTION()
//		void SetBatteryPercent(int32 value);
//
//	UFUNCTION()
//		void SetBatteryVisibility(bool value);
//
//public:
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		class UProgressBar* Battery;
//		
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		class UImage* BatteryImage;
};