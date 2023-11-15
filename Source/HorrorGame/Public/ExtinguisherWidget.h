// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "ExtinguisherWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UExtinguisherWidget : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void SetExtinguisherPercent(int32 value);

public:
	UPROPERTY()
		class UProgressBar* ExtBar;
};
