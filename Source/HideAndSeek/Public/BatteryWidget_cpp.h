// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "BatteryWidget_cpp.generated.h"


/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBatteryWidget_cpp : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
//	UUISlot_cpp(const FObjectInitializer& ObjectInitializer);
	UFUNCTION()
		void SetBatteryPercent(int32 value);

	UFUNCTION()
		void SetBatteryVisibility(bool value);

private:
	UPROPERTY()
		class UProgressBar* Battery;

	UPROPERTY()
		class UImage* BatteryImage;
};