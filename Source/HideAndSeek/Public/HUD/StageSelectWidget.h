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
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnClickChapOneButton();

	UFUNCTION(BlueprintCallable)
	void OnClickChapTwoButton();

	UFUNCTION(BlueprintCallable)
	void OnClickBackButton();

	UFUNCTION(BlueprintCallable)
	void OnMoveChapOne();

	UFUNCTION(BlueprintCallable)
	void OnMoveChapTwo();

	UFUNCTION(BlueprintCallable)
	void OnHoveredChapOneButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredChapTwoButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBackButton();

	UFUNCTION(BlueprintCallable)
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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* StageSampleImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ChapterPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavigationIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsButtonClicked; // 버튼 클릭 후, 다른 버튼으로 변경하지 못하도록 하기 위한 변수.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ClearedChapter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FClearData> ClearData;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	FWidgetAnimationDynamicEvent FadeoutWidgetAnimationEvent;
};
