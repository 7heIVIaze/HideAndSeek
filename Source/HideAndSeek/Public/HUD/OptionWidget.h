// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void OnClickPlaySettingButton();
	
	UFUNCTION(BlueprintCallable)
	void OnClickGraphicsSettingButton();

	UFUNCTION(BlueprintCallable)
	void OnClickBackButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredPlaySettingButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredGraphicsSettingButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBackButton();
		
	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlaySettingButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GraphicsSettingButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* OptionSelectBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* MenuLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* NewOptionAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* BackOptionAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;
};

