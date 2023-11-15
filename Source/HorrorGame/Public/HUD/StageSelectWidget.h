// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StageSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UStageSelectWidget : public UUserWidget
{
	GENERATED_BODY()

//public:
//	UStageSelectWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickChapOneButton();

	UFUNCTION()
		void OnClickChapTwoButton();

	UFUNCTION()
		void OnClickBackButton();

	UFUNCTION()
		void OnHoveredChapOneButton();

	UFUNCTION()
		void OnHoveredChapTwoButton();

	UFUNCTION()
		void OnHoveredBackButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY()
		class UButton* ChapOneButton;

	UPROPERTY()
		class UButton* ChapTwoButton;

	UPROPERTY()
		class UButton* BackButton;

	UPROPERTY()
		class UImage* LevelImg;

	UPROPERTY()
		int32 MenuNavigationIndex = 0;

	UPROPERTY()
		int32 MenuNumber = 3;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level_Image")
		TArray<UTexture2D*> LevelSample;

	//class UHorrorGameGameInstance* GameInstance;
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG_Widget")
//		TSubclassOf<UUserWidget> WidgetClass;
};
