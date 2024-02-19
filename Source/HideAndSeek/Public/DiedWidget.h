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
class HIDEANDSEEK_API UDiedWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickRestartButton();

	UFUNCTION()
		void OnClickExitButton();

	UFUNCTION()
		void OnHoveredRestartButton();

	UFUNCTION()
		void OnHoveredExitButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetArchiveGetText(FText inText);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UTextBlock* ArchiveGetText;

private:
	UPROPERTY()
		class UButton* RestartButton;

	UPROPERTY()
		class UButton* ExitButton;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 MenuNum = 2;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;
};
