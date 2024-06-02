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
	UFUNCTION()
		void OnExitButtonClick();

	UFUNCTION()
		void OnItemButtonClick();

	UFUNCTION()
		void OnDocumentButtonClick();

	UFUNCTION()
		void OnLetterButtonClick();

	UFUNCTION()
		void OnHintButtonClick();

	UFUNCTION()
		void OnExitButtonHovered();

	UFUNCTION()
		void OnItemButtonHovered();

	UFUNCTION()
		void OnDocumentButtonHovered();

	UFUNCTION()
		void OnLetterButtonHovered();

	UFUNCTION()
		void OnHintButtonHovered();

	UFUNCTION()
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

	UPROPERTY()
		int32 CurrentIndex;

	UPROPERTY()
		int32 NavIndex;

	UPROPERTY()
		int32 ItemIndex; // 현재 확인하려는 아이템의 번호

	UPROPERTY()
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
