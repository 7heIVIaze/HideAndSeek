// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DiedWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UDiedWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickRestartButton();

	UFUNCTION()
		void OnClickExitButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY()
		class UButton* RestartButton;

	UPROPERTY()
		class UButton* ExitButton;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 MenuNum = 2;
};
