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

	// Window Mode Buttons
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WindowButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* FullScreenButton;

	// Brightness Buttons
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAntiAliasing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShadowSetting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bMotionBlur;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString CurrentResolution;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SubMenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	GraphicsType CurrentMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UGameUserSettings* UserSetting;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* NewOptionAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* BackOptionAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EResolution ResolutionValue;

private:
	bool bWindowed;
	bool bFullScreen;
	
public:
	// Main Button Functions
	UFUNCTION(BlueprintCallable)
	void OnClickResolutionButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolutionButton();

	UFUNCTION(BlueprintCallable)
	void OnClickScreenSettingButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredScreenSettingButton();

	UFUNCTION(BlueprintCallable)
	void OnClickBrightButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBrightButton();

	UFUNCTION(BlueprintCallable)
	void OnClickAntiAliasingButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredAntiAliasingButton();

	UFUNCTION(BlueprintCallable)
	void OnClickShadowButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredShadowButton();

	UFUNCTION(BlueprintCallable)
	void OnClickMotionBlurButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredMotionBlurButton();

	// Resolution Buttons Function
	UFUNCTION(BlueprintCallable)
	void OnClickResolution1Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution1Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution2Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution2Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution3Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution3Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution4Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution4Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution5Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution5Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution6Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution6Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution7Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution7Button();

	UFUNCTION(BlueprintCallable)
	void OnClickResolution8Button();

	UFUNCTION(BlueprintCallable)
	void OnHoveredResolution8Button();

	// Window Mode Buttons Function
	UFUNCTION(BlueprintCallable)
	void OnClickWindowedButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredWindowedButton();

	UFUNCTION(BlueprintCallable)
	void OnClickFullScreenButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredFullScreenButton();

	UFUNCTION(BlueprintCallable)
	void SelectWindowMode(bool bWindowedMode, bool bFullScreenMode, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
	void SelectResolutionMode(EResolution inResolutionValue, int32 CurrentIndex);

	UFUNCTION(BlueprintCallable)
	void CheckCurrentResolution(const FString ForCheckResolution);

	// Brightness Setting
	UFUNCTION(BlueprintCallable)
	void SetBrightness(float Value);

	UFUNCTION(BlueprintCallable)
	void SetCurrentMode(GraphicsType ModeType);

	UFUNCTION(BlueprintCallable)
	void AnimationFinished();

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

	UFUNCTION(BlueprintCallable)
	void OnClickBackButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBackButton();

};
