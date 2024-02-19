// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "CutterWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class HIDEANDSEEK_API UCutterWidget : public UCustomUI
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public: // Functions
	UFUNCTION()
		void SetCutterPercent(int32 value);

	//void Refresh();

public: // Property
	UPROPERTY()
		class UImage* CutterImg;

	UPROPERTY()
		class UProgressBar* CutterBar;
};
