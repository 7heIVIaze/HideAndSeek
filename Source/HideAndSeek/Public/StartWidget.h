// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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

//public:
//	UStartWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnClickStartButton();

	UFUNCTION()
		void OnClickOptionButton();

	UFUNCTION()
		void OnClickExitButton();

	UFUNCTION()
		void OnClickArchiveButton();

	UFUNCTION()
		void OnHoveredStartButton();

	UFUNCTION()
		void OnHoveredOptionButton();

	UFUNCTION()
		void OnHoveredExitButton();

	UFUNCTION()
		void OnHoveredArchiveButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UPROPERTY()
		class UButton* StartButton;

	UPROPERTY()
		class UButton* OptionButton;
	 
	UPROPERTY()
		class UButton* ExitButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ArchiveBtn;

	UPROPERTY()
		class UVerticalBox* Menu;

private:
	UPROPERTY()
		int32 MenuNavigationIndex = 0;

	UPROPERTY()
		int32 MenuNumber;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;

//public:
//	UPROPERTY()
//		TSubclassOf<UUserWidget> WidgetClass;
};
