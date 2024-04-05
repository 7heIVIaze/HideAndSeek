// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
class HIDEANDSEEK_API UStageSelectWidget : public UUserWidget
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
		TArray<bool> CanButtonSelect;

	UPROPERTY()
		class UImage* LevelImg;

	UPROPERTY()
		class UVerticalBox* MenuBox;

	UPROPERTY()
		int32 MenuNavigationIndex = 0;

	UPROPERTY()
		int32 MenuNumber;

	//UPROPERTY()
		//int32 ButtonNum;

	UPROPERTY()
		int32 ClearedChapter;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level_Image")
		TArray<UTexture2D*> LevelSample;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> StageSelectSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;
};
