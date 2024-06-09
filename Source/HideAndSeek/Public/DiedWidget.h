// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* RestartButton;

		UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
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
