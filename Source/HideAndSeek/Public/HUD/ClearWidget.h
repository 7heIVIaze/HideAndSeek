// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public: // FUNCTION
	UFUNCTION(BlueprintCallable)
	void OnClickNextStage();

	UFUNCTION(BlueprintCallable)
	void OnClickBackToMain();

	UFUNCTION(BlueprintCallable)
	void OnHoveredNextStage();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBackToMain();

	UFUNCTION(BlueprintCallable)
	void SetClearTime(FString inClearTime);

	UFUNCTION(BlueprintCallable)
	void SetRecordBreakingText(bool inIsVisible);

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

public: // PROPERTY
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* NextStageButton; // 다음 스테이지 이동 버튼 => 다음 스테이지가 있으면 뜨고 없으면 안 뜨게

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackToMainButton; // 메인 화면 버튼

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString CurrentLevel; // 현재 레벨

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString LastLevel; // 엔딩 씬을 보여줄 마지막 레벨

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavigationIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHaveNextLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FText ClearedTime;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UTextBlock* ClearTime;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UTextBlock* RecordBreakingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;
};
