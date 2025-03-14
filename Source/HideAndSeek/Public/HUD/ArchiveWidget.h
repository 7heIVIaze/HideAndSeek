// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArchiveWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UArchiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
public:
	UFUNCTION(BlueprintCallable)
	void OnExitButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnItemButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnDocumentButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnLetterButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnHintButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnExitButtonHovered();

	UFUNCTION(BlueprintCallable)
	void OnItemButtonHovered();

	UFUNCTION(BlueprintCallable)
	void OnDocumentButtonHovered();

	UFUNCTION(BlueprintCallable)
	void OnLetterButtonHovered();

	UFUNCTION(BlueprintCallable)
	void OnHintButtonHovered();

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Document;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Letter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Hint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Exit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NavIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ItemIndex; // 현재 확인하려는 아이템의 번호

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 LanguageIndex; // 현재 언어 설정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> DetailWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> PauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> HintWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	bool bIsStartGameMode = true;
};
