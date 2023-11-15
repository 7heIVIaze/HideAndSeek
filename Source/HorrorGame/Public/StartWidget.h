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
class HORRORGAME_API UStartWidget : public UUserWidget
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
		void OnHoveredStartButton();

	UFUNCTION()
		void OnHoveredOptionButton();

	UFUNCTION()
		void OnHoveredExitButton();

	UFUNCTION()
		void UpdateButtonSlate();

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

private:
	UPROPERTY()
		class UButton* StartButton;

	UPROPERTY()
		class UButton* OptionButton;
	 
	UPROPERTY()
		class UButton* ExitButton;

	UPROPERTY()
		class UVerticalBox* Menu;


	UPROPERTY()
		int32 MenuNavigationIndex = 0;

	UPROPERTY()
		int32 MenuNumber = 3;

//public:
//	UPROPERTY()
//		TSubclassOf<UUserWidget> WidgetClass;
};
