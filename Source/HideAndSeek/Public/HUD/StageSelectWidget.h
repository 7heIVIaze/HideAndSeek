// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/Records.h"
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
	void OnMoveChapOne();

	UFUNCTION()
	void OnMoveChapTwo();

	/*UFUNCTION()
	void OnMoveBack();*/

	UFUNCTION()
	void OnHoveredChapOneButton();

	UFUNCTION()
	void OnHoveredChapTwoButton();

	UFUNCTION()
	void OnHoveredBackButton();

	UFUNCTION()
	void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ChapTwoText;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ChapOneButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ChapTwoButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

	/*UPROPERTY()
	TArray<bool> CanButtonSelect;*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* StageSampleImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ChapterPanel;

	UPROPERTY()
	int32 MenuNavigationIndex = 0;

	UPROPERTY()
	int32 MenuNumber;

	UPROPERTY()
	bool bIsButtonClicked; // 버튼 클릭 후, 다른 버튼으로 변경하지 못하도록 하기 위한 변수.

	UPROPERTY()
	int32 ClearedChapter;

	UPROPERTY()
	TArray<FClearData> ClearData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level_Image")
	TArray<UTexture2D*> LevelSample;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> StageSelectSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Fadeout;

	FWidgetAnimationDynamicEvent FadeoutWidgetAnimationEvent;
};
