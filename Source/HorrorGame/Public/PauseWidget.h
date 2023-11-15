// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickResumeButton();

	UFUNCTION()
		void OnClickRetryButton();

	UFUNCTION()
		void OnClickQuitButton();

	UFUNCTION()
		void OnHoveredResumeButton();

	UFUNCTION()
		void OnHoveredRetryButton();

	UFUNCTION()
		void OnHoveredQuitButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY()
		class UButton* RetryButton;

	UPROPERTY()
		class UButton* QuitButton;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 MenuNum = 3;
};
