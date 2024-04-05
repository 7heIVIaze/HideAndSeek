// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/GameSettingWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/CheckBox.h"
#include "HorrorGameGameInstance.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "Sound/SoundCue.h"

void UGameSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(NewOptionAnim);

	EndDelegate.BindDynamic(this, &UGameSettingWidget::AnimationFinished);
	BindToAnimationFinished(BackOptionAnim, EndDelegate);

	this->SetKeyboardFocus();

	// 위젯의 오브젝트 가져오기
	LanguageButton = Cast<UButton>(GetWidgetFromName(TEXT("Language")));
	EnglishButton = Cast<UButton>(GetWidgetFromName(TEXT("EnglishBtn")));
	KoreanButton = Cast<UButton>(GetWidgetFromName(TEXT("KoreanBtn")));
	VolumeButton = Cast<UButton>(GetWidgetFromName(TEXT("VolumeBtn")));
	VolumeSettingBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("VolumeBar")));
	TimerButton = Cast<UButton>(GetWidgetFromName(TEXT("TimerBtn")));
	TimerOnCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("TimerCheckBox")));
	CrossHairButton = Cast<UButton>(GetWidgetFromName(TEXT("CrossHairBtn")));
	CrossHairOnCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("CrossHairCheckBox")));
	MouseSensitiveButton = Cast<UButton>(GetWidgetFromName(TEXT("MouseSensitiveBtn")));
	SensitiveSettingBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SensitiveBar")));

	if (LanguageButton != nullptr)
	{
		LanguageButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickLanguageButton);
		LanguageButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredLanguageButton);
		LanguageButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredLanguageButton);
	}

	if (EnglishButton != nullptr)
	{
		EnglishButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickEnglishButton);
		EnglishButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredEnglishButton);
		EnglishButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredEnglishButton);
	}

	if (KoreanButton != nullptr)
	{
		KoreanButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickKoreanButton);
		KoreanButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredKoreanButton);
		KoreanButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredKoreanButton);
	}

	if (VolumeButton != nullptr)
	{
		VolumeButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickVolumeButton);
		VolumeButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredVolumeButton);
		VolumeButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredVolumeButton);
	}

	if (TimerButton != nullptr)
	{
		TimerButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickTimerButton);
		TimerButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredTimerButton);
		TimerButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredTimerButton);
	}

	if (CrossHairButton != nullptr)
	{
		CrossHairButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickCrossHairButton);
		CrossHairButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredCrossHairButton);
		CrossHairButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredCrossHairButton);
	}

	if (MouseSensitiveButton != nullptr)
	{
		MouseSensitiveButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickMouseSensitiveButton);
		MouseSensitiveButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredMouseSensitiveButton);
		MouseSensitiveButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredMouseSensitiveButton);
	}

	bIsEnglish = false;
	bIsKorean = false;
	bIsTimerOn = true;
	bIsCrossHairOn = true;

	FString CurrentCulture = UKismetInternationalizationLibrary::GetCurrentCulture();

	if (CurrentCulture.Equals(TEXT("ko")) || CurrentCulture.Equals(TEXT("ko-KR")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Current Culture is KR")));
		bIsKorean = true;
	}
	else if (CurrentCulture.Equals(TEXT("en")))
	{
		bIsEnglish = true;
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Culture is EN")));
	}

	// GameInstance로부터 타이머 설정 값과 조준점 설정값 가져오기(GameInstance는 실행 시 Save 데이터를 읽어옴)
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		bIsTimerOn = GameInstance->GetIsTimerOn();
		bIsCrossHairOn = GameInstance->GetIsCrossHairOn();
		Sensitive = GameInstance->GetMouseSensitive();
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Sensitivity: %f"), Sensitive));
		Sensitive = Sensitive / 5.f;
		Volume = GameInstance->GetVolume();
	}

//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Sensitive Percent: %f"), Sensitive));

	TimerOnCheckBox->SetIsChecked(bIsTimerOn);
	CrossHairOnCheckBox->SetIsChecked(bIsCrossHairOn);
	SensitiveSettingBar->SetPercent(Sensitive);
	VolumeSettingBar->SetPercent(Volume);

	SetCurrentMode(CurrentType::None);
}

void UGameSettingWidget::OnClickLanguageButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SetCurrentMode(CurrentType::LanguageSetting);
}

void UGameSettingWidget::OnHoveredLanguageButton()
{
	if (CurrentMode == CurrentType::None) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		MenuNavIndex = 0;
		UpdateButtonSlate();
		LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGameSettingWidget::OnUnhoveredLanguageButton()
{
	LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGameSettingWidget::OnClickEnglishButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("en"), true);
	bIsEnglish = true;
	bIsKorean = false;
	SetCurrentMode(CurrentType::None);
}

void UGameSettingWidget::OnHoveredEnglishButton()
{
	if (CurrentMode == CurrentType::LanguageSetting) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
		if (bIsEnglish)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}

void UGameSettingWidget::OnUnhoveredEnglishButton()
{
	if (bIsEnglish)
	{
		EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGameSettingWidget::OnClickKoreanButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("ko-KR"), true);
	bIsEnglish = false;
	bIsKorean = true;
	SetCurrentMode(CurrentType::None);
}

void UGameSettingWidget::OnHoveredKoreanButton()
{
	if (CurrentMode == CurrentType::LanguageSetting) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		SubMenuNavIndex = 1;
		UpdateButtonSlate();
		if (bIsKorean)
		{
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
	}
}

void UGameSettingWidget::OnUnhoveredKoreanButton()
{
	if (bIsKorean)
	{
		KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGameSettingWidget::OnClickVolumeButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SetCurrentMode(CurrentType::VolumeSetting);
}

void UGameSettingWidget::OnHoveredVolumeButton()
{
	if (CurrentMode == CurrentType::None) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		MenuNavIndex = 1;
		UpdateButtonSlate();
		VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGameSettingWidget::OnUnhoveredVolumeButton()
{
	VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGameSettingWidget::OnClickTimerButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	bIsTimerOn = !bIsTimerOn;

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		bool bResult = GameInstance->TimerOnSaveLogic(bIsTimerOn);
		if (bResult)
		{
			TimerOnCheckBox->SetIsChecked(bIsTimerOn);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Error to save Timer settings")));
		}
	}
}

void UGameSettingWidget::OnHoveredTimerButton()
{
	if (CurrentMode == CurrentType::None) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		MenuNavIndex = 2;
		UpdateButtonSlate();
		TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGameSettingWidget::OnUnhoveredTimerButton()
{
	TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGameSettingWidget::OnClickCrossHairButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	bIsCrossHairOn = !bIsCrossHairOn;

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		bool bResult = GameInstance->CrossHairOnSaveLogic(bIsCrossHairOn);
		if (bResult)
		{
			CrossHairOnCheckBox->SetIsChecked(bIsCrossHairOn);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Error to save CrossHair settings")));
		}
	}
}

void UGameSettingWidget::OnHoveredCrossHairButton()
{
	if (CurrentMode == CurrentType::None) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		MenuNavIndex = 3;
		UpdateButtonSlate();
		CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGameSettingWidget::OnUnhoveredCrossHairButton()
{
	CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGameSettingWidget::OnClickMouseSensitiveButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SetCurrentMode(CurrentType::MouseSensitiveSetting);
}

void UGameSettingWidget::OnHoveredMouseSensitiveButton()
{
	if (CurrentMode == CurrentType::None) // 현재 아무 것도 선택되지 않은 상태일 때만
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		MenuNavIndex = 4;
		UpdateButtonSlate();
		MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGameSettingWidget::OnUnhoveredMouseSensitiveButton()
{
	MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGameSettingWidget::SetCurrentMode(CurrentType ModeType)
{
	CurrentMode = ModeType;

	if (ModeType == CurrentType::None)
	{
		MenuNumber = 5;
	}
	else if (ModeType == CurrentType::LanguageSetting)
	{
		MenuNumber = 2;
	}
	UpdateButtonSlate();
}

void UGameSettingWidget::OnChangeGlobalVolume(float inValue)
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	Volume += inValue;
	Volume = FMath::Clamp(Volume, 0.f, 1.f); // 볼륨의 최대 크기가 0에서 1 사이로 설정되도록 함
	VolumeSettingBar->SetPercent(Volume);

	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}

void UGameSettingWidget::OnChangeMouseSensitive(float inValue)
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	Sensitive += inValue;
	Sensitive = FMath::Clamp(Sensitive, 0.1f, 1.f);
	SensitiveSettingBar->SetPercent(Sensitive);
}

void UGameSettingWidget::AnimationFinished()
{
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

void UGameSettingWidget::UpdateButtonSlate()
{
	if (CurrentMode == CurrentType::None)
	{
		switch (MenuNavIndex)
		{
		case 0:
			LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			break;
		case 1:
			LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			break;
		case 2:
			LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			break;
		case 3:
			LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			break;
		case 4:
			LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
	}
	else
	{
		LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}

	if (CurrentMode == CurrentType::LanguageSetting)
	{
		if (bIsEnglish)
		{
			if (SubMenuNavIndex == 0)
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
		else
		{
			if (SubMenuNavIndex == 0)
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
			else
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			}
		}

		if (bIsKorean)
		{
			if (SubMenuNavIndex == 1)
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			else
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
		else
		{
			if (SubMenuNavIndex == 1)
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
			else
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			}
		}
	}
	else
	{
		if (bIsEnglish)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
		else if (bIsKorean)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
}

FReply UGameSettingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		if (CurrentMode == CurrentType::None)
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickLanguageButton();
				UpdateButtonSlate();
				break;
			case 1:
				OnClickVolumeButton();
				UpdateButtonSlate();
				break;
			case 2:
				OnClickTimerButton();
				UpdateButtonSlate();
				break;
			case 3:
				OnClickCrossHairButton();
				UpdateButtonSlate();
				break;
			case 4:
				OnClickMouseSensitiveButton();
				UpdateButtonSlate();
				break;
			}
		}

		else if (CurrentMode == CurrentType::LanguageSetting)
		{
			switch (SubMenuNavIndex)
			{
			case 0:
				OnClickEnglishButton();
				break;
			case 1:
				OnClickKoreanButton();
				break;
			}
			SetCurrentMode(CurrentType::None);
		}

		else if (CurrentMode == CurrentType::VolumeSetting)
		{
			if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				if (SoundMix != nullptr && SoundClass != nullptr)
				{
					UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, Volume);
					UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
					bool SaveResult = GameInstance->VolumeSaveLogic(Volume);
				}
			}

			SetCurrentMode(CurrentType::None);
		}

		else if (CurrentMode == CurrentType::MouseSensitiveSetting)
		{
			if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				Sensitive = 5.f * Sensitive;

				bool SaveResult = GameInstance->MouseSensitiveSaveLogic(Sensitive);
			}
			SetCurrentMode(CurrentType::None);
		}
	}
	else
	{
		if (CurrentMode == CurrentType::None)
		{
			if (KeyType == "S" || KeyType == "Down")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				MenuNavIndex++;
				if (MenuNavIndex >= MenuNumber)
				{
					MenuNavIndex = 0;
				}
			}
			else if (KeyType == "W" || KeyType == "Up")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				MenuNavIndex--;
				if (MenuNavIndex < 0)
				{
					MenuNavIndex = MenuNumber - 1;
				}
			}
			else if (KeyType == "A" || KeyType == "Left")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				PlayAnimation(BackOptionAnim);
			}
		}

		else if (CurrentMode == CurrentType::LanguageSetting)
		{
			if (KeyType == "D" || KeyType == "Right")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				SubMenuNavIndex++;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex = 0;
				}
			}
			else if (KeyType == "A" || KeyType == "Left")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
		}

		else if (CurrentMode == CurrentType::VolumeSetting)
		{
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				OnChangeGlobalVolume(-0.01);
			}
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				OnChangeGlobalVolume(0.01);
			}
		}

		else if (CurrentMode == CurrentType::MouseSensitiveSetting)
		{
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				OnChangeMouseSensitive(-0.01);
			}
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				OnChangeMouseSensitive(0.01);
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}