// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/Records.h"
#include "GraphicsOption.generated.h"

UENUM(BlueprintType)
enum class GraphicsType : uint8
{
	Option UMETA(DisplayName = "Option"),
	None UMETA(DisplayName = "None"),
	Resolution UMETA(DisplayName = "ScreenResolution"),
	ScreenSetting UMETA(DisplayName = "ScreenSetting"),
	Brightness UMETA(DisplayName = "Brightness"),
	/*AntiAliasing UMETA(DisplayName = "AntiAliasing"),
	ShadowSetting UMETA(DisplayName = "ShadowSetting"),
	MotionBlur UMETA(DisplayName = "MotionBlur"),*/
};
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UGraphicsOption : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	// Main Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* ResolutionButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* ScreenSettingButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* BrightButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* AntiAliasingButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* ShadowButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* MotionBlurButton;

	// Resolution Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution1Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution2Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution3Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution4Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution5Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution6Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution7Button;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution8Button;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution9Btn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* Resolution10Btn;*/

		// Window Mode Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* WindowButton;

	/*UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* WinFullBtn;*/

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UButton* FullScreenButton;

	// Brightness Buttons
	UPROPERTY()
		float Brightness;

	// Check Boxes
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCheckBox* AntiCheckbox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCheckBox* ShadowCheckbox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UCheckBox* MotionBlurCheckbox;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UVerticalBox* GraphicsSettingPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UUniformGridPanel* ResolutionType;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UHorizontalBox* WindowModePanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UProgressBar* BrightBar;

	UPROPERTY()
		bool bIsAntiAliasing;

	UPROPERTY()
		bool bShadowSetting;

	UPROPERTY()
		bool bMotionBlur;

	UPROPERTY()
		FString CurrentResolution;

	UPROPERTY()
		int32 MenuNavIndex = 0;

	UPROPERTY()
		int32 SubMenuNavIndex = 0;

	UPROPERTY()
		GraphicsType CurrentMode;

	UPROPERTY()
		class UGameUserSettings* UserSetting;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* NewOptionAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* BackOptionAnim;

	UPROPERTY()
		int32 MenuNumber;

	UPROPERTY()
		FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* BackButton;

	UPROPERTY()
	EResolution ResolutionValue;

private:
	bool bWindowed;
	bool bFullScreen;
	/*bool bResolution1;
	bool bResolution2;
	bool bResolution3;
	bool bResolution4;
	bool bResolution5;
	bool bResolution6;
	bool bResolution7;
	bool bResolution8;*/

public:
	// Main Button Functions
	UFUNCTION()
		void OnClickResolutionButton();

	UFUNCTION()
		void OnHoveredResolutionButton();

	/*UFUNCTION()
		void OnUnhoveredResolutionButton();*/

	UFUNCTION()
		void OnClickScreenSettingButton();

	UFUNCTION()
		void OnHoveredScreenSettingButton();

	//UFUNCTION()
	//	void OnUnhoveredWindowModeButton();

	UFUNCTION()
		void OnClickBrightButton();

	UFUNCTION()
		void OnHoveredBrightButton();

	/*UFUNCTION()
		void OnUnhoveredBrightButton();*/

	UFUNCTION()
		void OnClickAntiAliasingButton();

	UFUNCTION()
		void OnHoveredAntiAliasingButton();

	//UFUNCTION()
	//	void OnUnhoveredAntiAliasingButton();

	UFUNCTION()
		void OnClickShadowButton();

	UFUNCTION()
		void OnHoveredShadowButton();

	/*UFUNCTION()
		void OnUnhoveredShadowButton();*/

	UFUNCTION()
		void OnClickMotionBlurButton();

	UFUNCTION()
		void OnHoveredMotionBlurButton();

	/*UFUNCTION()
		void OnUnhoveredMotionBlurButton();*/

	// Resolution Buttons Function
	UFUNCTION()
		void OnClickResolution1Button();

	UFUNCTION()
		void OnHoveredResolution1Button();

	//UFUNCTION()
	//	void OnUnhoveredResolution1Button();

	UFUNCTION()
		void OnClickResolution2Button();

	UFUNCTION()
		void OnHoveredResolution2Button();

	/*UFUNCTION()
		void OnUnhoveredResolution2Button();*/

	UFUNCTION()
		void OnClickResolution3Button();

	UFUNCTION()
		void OnHoveredResolution3Button();

	//UFUNCTION()
	//	void OnUnhoveredResolution3Button();

	UFUNCTION()
		void OnClickResolution4Button();

	UFUNCTION()
		void OnHoveredResolution4Button();

	//UFUNCTION()
	//	void OnUnhoveredResolution4Button();

	UFUNCTION()
		void OnClickResolution5Button();

	UFUNCTION()
		void OnHoveredResolution5Button();

	//UFUNCTION()
	//	void OnUnhoveredResolution5Button();

	UFUNCTION()
		void OnClickResolution6Button();

	UFUNCTION()
		void OnHoveredResolution6Button();

	/*UFUNCTION()
		void OnUnhoveredResolution6Button();*/

	UFUNCTION()
		void OnClickResolution7Button();

	UFUNCTION()
		void OnHoveredResolution7Button();

	/*UFUNCTION()
		void OnUnhoveredResolution7Button();*/

	UFUNCTION()
		void OnClickResolution8Button();

	UFUNCTION()
		void OnHoveredResolution8Button();

	/*UFUNCTION()
		void OnUnhoveredResolution8Button();*/

	/*UFUNCTION()
		void OnClickResolution9Button();

	UFUNCTION()
		void OnHoveredResolution9Button();

	UFUNCTION()
		void OnUnhoveredResolution9Button();

	UFUNCTION()
		void OnClickResolution10Button();

	UFUNCTION()
		void OnHoveredResolution10Button();

	UFUNCTION()
		void OnUnhoveredResolution10Button(); */

		// Window Mode Buttons Function
	UFUNCTION()
		void OnClickWindowedButton();

	UFUNCTION()
		void OnHoveredWindowedButton();

	/*UFUNCTION()
		void OnUnhoveredWindowedButton();*/

	/*UFUNCTION()
		void OnClickWinFullButton();

	UFUNCTION()
		void OnHoveredWinFullButton();

	UFUNCTION()
		void OnUnhoveredWinFullButton(); */

	UFUNCTION()
		void OnClickFullScreenButton();

	UFUNCTION()
		void OnHoveredFullScreenButton();

	//UFUNCTION()
	//	void OnUnhoveredFullScreenButton();

	/*UFUNCTION(BlueprintCallable)
		void GraphicMenu(int32 CurrentIndex);*/

	UFUNCTION(BlueprintCallable)
		void SelectWindowMode(bool bWindowedMode, bool bFullScreenMode, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
		/*void SelectResolutionMode(bool bResolution1Mode, bool bResolution2Mode, bool bResolution3Mode, bool bResolution4Mode,
			bool bResolution5Mode, bool bResolution6Mode, bool bResolution7Mode, bool bResolution8Mode, int32 CurrentIndex);*/
		void SelectResolutionMode(EResolution inResolutionValue, int32 CurrentIndex);

	UFUNCTION()
		void CheckCurrentResolution(const FString ForCheckResolution);

	// Brightness Setting
	UFUNCTION(BlueprintCallable)
		void SetBrightness(float Value);

	UFUNCTION()
		void SetCurrentMode(GraphicsType ModeType);

	UFUNCTION()
		void AnimationFinished();

	UFUNCTION()
		void UpdateButtonSlate();

	UFUNCTION()
		void OnClickBackButton();

	UFUNCTION()
		void OnHoveredBackButton();

	/*UFUNCTION()
		void OnUnhoveredBackButton();*/
};
