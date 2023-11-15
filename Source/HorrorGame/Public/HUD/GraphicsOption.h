// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GraphicsOption.generated.h"

//UENUM(BlueprintType)
//enum class GraphicsType : uint8
//{
//	Option UMETA(DisplayName = "Option"),
//	None UMETA(DisplayName = "None"),
//	Resolution UMETA(DisplayName = "ScreenResolution"),
//	Window UMETA(DisplayName = "WindowMode"),
//	Brightness UMETA(DisplayName = "Brightness"),
//	/*AntiAliasing UMETA(DisplayName = "AntiAliasing"),
//	ShadowSetting UMETA(DisplayName = "ShadowSetting"),
//	MotionBlur UMETA(DisplayName = "MotionBlur"),*/
//};
/**
 * 
 */
UCLASS()
class HORRORGAME_API UGraphicsOption : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	// Main Buttons
	UPROPERTY()
		class UButton* ResolutionButton;

	UPROPERTY()
		class UButton* WindowModeButton;

	UPROPERTY()
		class UButton* BrightnessButton;

	UPROPERTY()
		class UButton* AntiAliasingButton;

	UPROPERTY()
		class UButton* ShadowButton;

	UPROPERTY()
		class UButton* MotionBlurButton;

	// Resolution Buttons
	UPROPERTY()
		class UButton* Resolution1Button;

	UPROPERTY()
		class UButton* Resolution2Button;

	UPROPERTY()
		class UButton* Resolution3Button;

	UPROPERTY()
		class UButton* Resolution4Button;

	UPROPERTY()
		class UButton* Resolution5Button;

	UPROPERTY()
		class UButton* Resolution6Button;

	UPROPERTY()
		class UButton* Resolution7Button;

	UPROPERTY()
		class UButton* Resolution8Button;

	UPROPERTY()
		class UButton* Resolution9Button;

	UPROPERTY()
		class UButton* Resolution10Button;

	// Window Mode Buttons
	UPROPERTY()
		class UButton* WindowedButton;

	UPROPERTY()
		class UButton* WinFullButton;

	UPROPERTY()
		class UButton* FullScreenButton;

	// Brightness Buttons
	UPROPERTY()
		int32 Brightness;
	
	// Check Boxes
	UPROPERTY()
		class UCheckBox* AntiAliasBox;

	UPROPERTY()
		class UCheckBox* ShadowBox;

	UPROPERTY()
		class UCheckBox* MotionBlurBox;

	UPROPERTY()
		class UVerticalBox* MainBox;

	UPROPERTY()
		class UUniformGridPanel* ResolutionPanel;

	UPROPERTY()
		class UHorizontalBox* WindowModeBox;

	UPROPERTY()
		class UProgressBar* BrightnessBar;

	UPROPERTY()
		bool bIsAntiAliasing;

	UPROPERTY()
		bool bShadowSetting;

	UPROPERTY()
		bool bMotionBlur;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 SubMenuNavIndex = 0;

	UPROPERTY()
		int32 MenuNumber;

	UPROPERTY()
		GraphicsType CurrentMode;

public:
	// Main Button Functions
	UFUNCTION()
		void OnClickResolutionButton();

	UFUNCTION()
		void OnHoveredResolutionButton();

	UFUNCTION()
		void OnClickWindowModeButton();

	UFUNCTION()
		void OnHoveredWindowModeButton();

	UFUNCTION()
		void OnClickBrightButton();

	UFUNCTION()
		void OnHoveredBrightButton();

	UFUNCTION()
		void OnClickAntiAliasingButton();

	UFUNCTION()
		void OnHoveredAntiAliasingButton();

	UFUNCTION()
		void OnClickShadowButton();

	UFUNCTION()
		void OnHoveredShadowButton();

	UFUNCTION()
		void OnClickMotionBlurButton();

	UFUNCTION()
		void OnHoveredMotionBlurButton();

	// Resolution Buttons Function
	UFUNCTION()
		void OnClickResolution1Button();

	UFUNCTION()
		void OnHoveredResolution1Button();

	UFUNCTION()
		void OnClickResolution2Button();

	UFUNCTION()
		void OnHoveredResolution2Button();

	UFUNCTION()
		void OnClickResolution3Button();

	UFUNCTION()
		void OnHoveredResolution3Button();

	UFUNCTION()
		void OnClickResolution4Button();

	UFUNCTION()
		void OnHoveredResolution4Button();

	UFUNCTION()
		void OnClickResolution5Button();

	UFUNCTION()
		void OnHoveredResolution5Button();

	UFUNCTION()
		void OnClickResolution6Button();

	UFUNCTION()
		void OnHoveredResolution6Button();

	UFUNCTION()
		void OnClickResolution7Button();

	UFUNCTION()
		void OnHoveredResolution7Button();

	UFUNCTION()
		void OnClickResolution8Button();

	UFUNCTION()
		void OnHoveredResolution8Button();

	UFUNCTION()
		void OnClickResolution9Button();

	UFUNCTION()
		void OnHoveredResolution9Button();

	UFUNCTION()
		void OnClickResolution10Button();

	UFUNCTION()
		void OnHoveredResolution10Button();

	// Window Mode Buttons Function
	UFUNCTION()
		void OnClickWindowedButton();

	UFUNCTION()
		void OnHoveredWindowedButton();

	UFUNCTION()
		void OnClickWinFullButton();

	UFUNCTION()
		void OnHoveredWinFullButton();

	UFUNCTION()
		void OnClickFullScreenButton();

	UFUNCTION()
		void OnHoveredFullScreenButton();

	// Brightness Setting
	UFUNCTION()
		void SetBrightness(int32 value);

	UFUNCTION()
		void UpdateButtonSlate();

	UFUNCTION()
		void SetCurrentMode(GraphicsType ModeType);
};
