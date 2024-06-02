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
	UFUNCTION()
	void OnClickLanguageButton();

	UFUNCTION()
	void OnHoveredLanguageButton();

	/*UFUNCTION()
	void OnUnhoveredLanguageButton();*/

	UFUNCTION()
	void OnClickEnglishButton();

	UFUNCTION()
	void OnHoveredEnglishButton();

	//UFUNCTION()
	//void OnUnhoveredEnglishButton();

	UFUNCTION()
	void OnClickKoreanButton();

	UFUNCTION()
	void OnHoveredKoreanButton();

	//UFUNCTION()
	//void OnUnhoveredKoreanButton();

	UFUNCTION()
	void OnClickVolumeButton();

	UFUNCTION()
	void OnHoveredVolumeButton();

	/*UFUNCTION()
	void OnUnhoveredVolumeButton();*/

	UFUNCTION(BlueprintCallable)
	void OnChangeGlobalVolume(float inValue);

	UFUNCTION(BlueprintCallable)
	void OnClickTimerButton();

	UFUNCTION()
	void OnHoveredTimerButton();

	/*UFUNCTION()
	void OnUnhoveredTimerButton();*/

	UFUNCTION(BlueprintCallable)
	void OnClickCrossHairButton();

	UFUNCTION()
	void OnHoveredCrossHairButton();

	/*UFUNCTION()
	void OnUnhoveredCrossHairButton();*/

	UFUNCTION()
	void OnClickMouseSensitiveButton();

	UFUNCTION()
	void OnHoveredMouseSensitiveButton();

	/*UFUNCTION()
	void OnUnhoveredMouseSensitiveButton();*/

	UFUNCTION(BlueprintCallable)
	void OnChangeMouseSensitive(float inValue);

	UFUNCTION()
	void UpdateButtonSlate();

	UFUNCTION()
	void SetCurrentMode(CurrentType ModeType);

	UFUNCTION()
	void AnimationFinished();

	UFUNCTION()
	void OnClickBackButton();

	UFUNCTION()
	void OnHoveredBackButton();

	/*UFUNCTION()
	void OnUnhoveredBackButton();*/

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

	UPROPERTY()
	int32 MenuNavIndex = 0;

	UPROPERTY()
	int32 SubMenuNavIndex = 0;

	UPROPERTY()
	int32 MenuNumber;

	UPROPERTY()
	CurrentType CurrentMode;

	UPROPERTY()
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
	bool bIsEnglish;
	bool bIsKorean;
	bool bIsTimerOn;
	bool bIsCrossHairOn;

	float Volume;
	float Sensitive;
	FOptionSettings OptionSetting;
};
