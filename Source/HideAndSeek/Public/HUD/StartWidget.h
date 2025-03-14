// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnClickStartButton();

	UFUNCTION(BlueprintCallable)
	void OnClickOptionButton();

	UFUNCTION(BlueprintCallable)
	void OnClickExitButton();

	UFUNCTION(BlueprintCallable)
	void OnClickArchiveButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredStartButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredOptionButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredExitButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredArchiveButton();

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* OptionButton;
	 
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ArchiveBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* Menu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

private:
	UPROPERTY()
	int32 MenuNavigationIndex = 0;

	UPROPERTY()
	int32 MenuNumber;
};
