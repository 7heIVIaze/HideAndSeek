// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
class HIDEANDSEEK_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickResumeButton();

	UFUNCTION()
		void OnClickArchiveButton();

	UFUNCTION()
		void OnClickRetryButton();

	UFUNCTION()
		void OnClickQuitButton();

	UFUNCTION()
		void OnHoveredResumeButton();

	UFUNCTION()
		void OnHoveredArchiveButton();

	UFUNCTION()
		void OnHoveredRetryButton();

	UFUNCTION()
		void OnHoveredQuitButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> ArchiveWidget;

public:
	UPROPERTY()
		class UButton* ResumeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ArchiveButton;

	UPROPERTY()
		class UButton* RetryButton;

	UPROPERTY()
		class UButton* QuitButton;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 MenuNum = 4;
};
