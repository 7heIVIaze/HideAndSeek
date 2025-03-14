// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComponentAction/Records.h"
#include "GameSettingWidget.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class CurrentType : uint8
{
	None UMETA(DisplayName = "None"),
	LanguageSetting UMETA(DisplayName = "LanguageSetting"),
	VolumeSetting UMETA(DisplayName = "VolumeSetting"),
	MouseSensitiveSetting UMETA(DisplayName = "MouseSensitiveSetting"),
};

UCLASS()
class HIDEANDSEEK_API UGameSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void OnClickLanguageButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredLanguageButton();

	UFUNCTION(BlueprintCallable)
	void OnClickEnglishButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredEnglishButton();

	UFUNCTION(BlueprintCallable)
	void OnClickKoreanButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredKoreanButton();

	UFUNCTION(BlueprintCallable)
	void OnClickVolumeButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredVolumeButton();

	UFUNCTION(BlueprintCallable)
	void OnChangeGlobalVolume(float inValue);

	UFUNCTION(BlueprintCallable)
	void OnClickTimerButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredTimerButton();

	UFUNCTION(BlueprintCallable)
	void OnClickCrossHairButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredCrossHairButton();

	UFUNCTION(BlueprintCallable)
	void OnClickMouseSensitiveButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredMouseSensitiveButton();

	UFUNCTION(BlueprintCallable)
	void OnChangeMouseSensitive(float inValue);

	UFUNCTION(BlueprintCallable)
	void UpdateButtonSlate();

	UFUNCTION(BlueprintCallable)
	void SetCurrentMode(CurrentType ModeType);

	UFUNCTION(BlueprintCallable)
	void AnimationFinished();

	UFUNCTION(BlueprintCallable)
	void OnClickBackButton();

	UFUNCTION(BlueprintCallable)
	void OnHoveredBackButton();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* LanguageButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* EnglishButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* KoreanButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* VolumeButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* VolumeSettingBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* TimerButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* TimerOnCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CrossHairButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* CrossHairOnCheckBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* MouseSensitiveButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* SensitiveSettingBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* NewOptionAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* BackOptionAnim;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SubMenuNavIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 MenuNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	CurrentType CurrentMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundMix> SoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundClass> SoundClass;

private:
	UPROPERTY()
	FOptionSettings OptionSetting;
};
