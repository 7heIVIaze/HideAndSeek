// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OptionWidget.generated.h"

// Deprecated
//UENUM(BlueprintType)
//enum class OptionType : uint8
//{
//	None UMETA(DisplayName = "None"),
//	PlayerSetting UMETA(DisplayName = "PlayerSetting"),
//	GraphicSetting UMETA(DisplayName = "GraphicSetting"),
//	KeySetting UMETA(DisplayName = "KeySetting"),
//};

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
	UFUNCTION()
		void OnClickPlaySettingButton();
	
	UFUNCTION()
		void OnClickGraphicsSettingButton();

	//UFUNCTION()
	//	void OnClickKeyMappingButton();

	UFUNCTION()
		void OnClickBackButton();

	UFUNCTION()
		void OnHoveredPlaySettingButton();

	UFUNCTION()
		void OnHoveredGraphicsSettingButton();

	/*UFUNCTION()
		void OnHoveredKeyMappingButton();*/

	UFUNCTION()
		void OnHoveredBackButton();
		
	UFUNCTION()
		void UpdateButtonSlate();

	/*UFUNCTION()
		void SetCurrentMode(OptionType ModeType);*/

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* PlaySettingButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* GraphicsSettingButton;

	//UPROPERTY()
	//	class UButton* KeyMappingButton;

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

	UPROPERTY()
	int32 MenuNavIndex = 0;

	/*UPROPERTY()
		int32 SettingNavIndex = 0;*/

	UPROPERTY()
	int32 MenuNumber;

	/*UPROPERTY()
	OptionType CurrentMode;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

};

