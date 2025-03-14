// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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

	UFUNCTION(BlueprintCallable)
	void OnClickResumeButton();

	UFUNCTION(BlueprintCallable)
	void OnClickArchiveButton();

	UFUNCTION(BlueprintCallable)
	void OnClickRetryButton();

	UFUNCTION(BlueprintCallable)
	void OnClickQuitButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResumeButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredArchiveButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredRetryButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredQuitButton();

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> ArchiveWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ArchiveButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RetryButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuitButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNum = 4;
};
