// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/GameSettingWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/CheckBox.h"
#include "HorrorGameGameInstance.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "Sound/SoundCue.h"

// 생성자
void UGameSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(NewOptionAnim);

	EndDelegate.BindDynamic(this, &UGameSettingWidget::AnimationFinished);
	BindToAnimationFinished(BackOptionAnim, EndDelegate);

	this->SetKeyboardFocus();

	// GameInstance로부터 타이머 설정 값과 조준점 설정값 가져오기(GameInstance는 실행 시 Save 데이터를 읽어옴)
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		OptionSetting.bIsTimerOn = GameInstance->GetIsTimerOn();
		OptionSetting.bIsCrossHairOn = GameInstance->GetIsCrossHairOn();
		OptionSetting.MouseSensitive = GameInstance->GetMouseSensitive();
		OptionSetting.Volume = GameInstance->GetVolume();
		OptionSetting.Language = GameInstance->GetCurrentLanguage();
	}

	// 위젯의 오브젝트 가져오기
	/*LanguageButton = Cast<UButton>(GetWidgetFromName(TEXT("Language")));
	EnglishButton = Cast<UButton>(GetWidgetFromName(TEXT("EnglishBtn")));
	KoreanButton = Cast<UButton>(GetWidgetFromName(TEXT("KoreanBtn")));
	VolumeButton = Cast<UButton>(GetWidgetFromName(TEXT("VolumeBtn")));
	VolumeSettingBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("VolumeBar")));
	TimerButton = Cast<UButton>(GetWidgetFromName(TEXT("TimerBtn")));
	TimerOnCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("TimerCheckBox")));
	CrossHairButton = Cast<UButton>(GetWidgetFromName(TEXT("CrossHairBtn")));
	CrossHairOnCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("CrossHairCheckBox")));
	MouseSensitiveButton = Cast<UButton>(GetWidgetFromName(TEXT("MouseSensitiveBtn")));
	SensitiveSettingBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("SensitiveBar")));*/

	// 각 버튼 별로, 클릭, 마우스 호버 시 작동할 함수를 바인딩 해줌.
	if (LanguageButton != nullptr)
	{
		LanguageButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickLanguageButton);
		LanguageButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredLanguageButton);
		//LanguageButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredLanguageButton);
	}

	if (EnglishButton != nullptr)
	{
		EnglishButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickEnglishButton);
		EnglishButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredEnglishButton);
		//EnglishButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredEnglishButton);
		
		// 저장된 언어 설정이 영어면, 버튼 색을 붉지만 살짝 투명하게 설정함.
		if (OptionSetting.Language == ELanguage::LANG_En)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}

	if (KoreanButton != nullptr)
	{
		KoreanButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickKoreanButton);
		KoreanButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredKoreanButton);
		//KoreanButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredKoreanButton);
		
		// 저장된 언어 설정이 한국어이면, 버튼 색을 붉지만 살짝 투명하게 설정함.
		if (OptionSetting.Language == ELanguage::LANG_Ko)
		{
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}

	if (VolumeButton != nullptr)
	{
		VolumeButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickVolumeButton);
		VolumeButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredVolumeButton);
		//VolumeButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredVolumeButton);
	}

	if (TimerButton != nullptr)
	{
		TimerButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickTimerButton);
		TimerButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredTimerButton);
		//TimerButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredTimerButton);
	}

	if (CrossHairButton != nullptr)
	{
		CrossHairButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickCrossHairButton);
		CrossHairButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredCrossHairButton);
		//CrossHairButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredCrossHairButton);
	}

	if (MouseSensitiveButton != nullptr)
	{
		MouseSensitiveButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickMouseSensitiveButton);
		MouseSensitiveButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredMouseSensitiveButton);
		//MouseSensitiveButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredMouseSensitiveButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UGameSettingWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UGameSettingWidget::OnHoveredBackButton);
		//BackButton->OnUnhovered.AddDynamic(this, &UGameSettingWidget::OnUnhoveredBackButton);
	}
	TimerOnCheckBox->SetIsChecked(OptionSetting.bIsTimerOn);
	CrossHairOnCheckBox->SetIsChecked(OptionSetting.bIsCrossHairOn);
	SensitiveSettingBar->SetPercent(OptionSetting.MouseSensitive / 5.f);
	VolumeSettingBar->SetPercent(OptionSetting.Volume / 2.f);

	SetCurrentMode(CurrentType::None);
	UpdateButtonSlate();
}

// 언어 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickLanguageButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		// 현재 위젯 모드를 언어 설정 모드로 변경함.
		SetCurrentMode(CurrentType::LanguageSetting);
	}
}

// 언어 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGameSettingWidget::OnHoveredLanguageButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None) 
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 0;
		UpdateButtonSlate();
		// LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredLanguageButton()
//{
//	LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 영어 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickEnglishButton()
{
	// 현재 위젯 모드가 언어 설정 모드인 경우에만 작동하도록 설정.
	if (CurrentMode == CurrentType::LanguageSetting)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 언어를 영어로 변경하고, 이를 저장함.
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("en"), true);
		OptionSetting.Language = ELanguage::LANG_En;
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			bool bResult = GameInstance->OptionSettingSaveLogic(OptionSetting);
		}

		// 그 후 위젯 모드를 최초 모드로 변경함.
		SetCurrentMode(CurrentType::None);
	}
}

// 영어 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGameSettingWidget::OnHoveredEnglishButton()
{ 
	// 현재 위젯 모드가 언어 설정 모드인 경우에만 작동하도록 설정.
	if (CurrentMode == CurrentType::LanguageSetting)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
		/*if (OptionSetting.Language == ELanguage::LANG_En)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}*/
	}
}

//void UGameSettingWidget::OnUnhoveredEnglishButton()
//{
//	if (OptionSetting.Language == ELanguage::LANG_En)
//	{
//		EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 한국어 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickKoreanButton()
{
	// 현재 위젯 모드가 언어 설정 모드인 경우에만 작동하도록 설정.
	if (CurrentMode == CurrentType::LanguageSetting)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 언어를 한국어로 변경하고, 이를 저장함.
		UKismetInternationalizationLibrary::SetCurrentCulture(TEXT("ko-KR"), true);
		OptionSetting.Language = ELanguage::LANG_Ko;
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			bool bResult = GameInstance->OptionSettingSaveLogic(OptionSetting);
		}

		// 그 후 위젯 모드를 최초 모드로 변경함.
		SetCurrentMode(CurrentType::None);
	}
}

// 한국어 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGameSettingWidget::OnHoveredKoreanButton()
{
	// 현재 위젯 모드가 언어 설정 모드인 경우에만 작동하도록 설정.
	if (CurrentMode == CurrentType::LanguageSetting)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 1;
		UpdateButtonSlate();
		/*if (OptionSetting.Language == ELanguage::LANG_Ko)
		{
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}*/
	}
}

//void UGameSettingWidget::OnUnhoveredKoreanButton()
//{
//	if (OptionSetting.Language == ELanguage::LANG_Ko)
//	{
//		KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 볼륨 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickVolumeButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 위젯 모드를 볼륨 설정 모드로 변경함.
		SetCurrentMode(CurrentType::VolumeSetting);
	}
}

// 볼륨 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnHoveredVolumeButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 1로 변경하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 1;
		UpdateButtonSlate();
		// VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredVolumeButton()
//{
//	VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 타이머 설정 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickTimerButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 설정된 타이머 설정값을 반전시켜 저장함.
		OptionSetting.bIsTimerOn = !OptionSetting.bIsTimerOn;

		// 그 후 세이브 데이터에 영구히 저장시킴.
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			bool bResult = GameInstance->OptionSettingSaveLogic(OptionSetting);
			
			// 저장에 성공하면 체크 박스에 표시해 둠.
			if (bResult)
			{
				TimerOnCheckBox->SetIsChecked(OptionSetting.bIsTimerOn);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Error to save Timer settings"));
			}
		}
	}
}

// 타이머 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGameSettingWidget::OnHoveredTimerButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 2;
		UpdateButtonSlate();
		// TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredTimerButton()
//{
//	TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 조준점 설정 버튼을 클릭하면 작동할 함수.
void UGameSettingWidget::OnClickCrossHairButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 설정된 조준점 설정 값을 반전시킴.
		OptionSetting.bIsCrossHairOn = !OptionSetting.bIsCrossHairOn;

		// 그 후 설정 값을 세이브 데이터에 영구히 저장함.
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			bool bResult = GameInstance->OptionSettingSaveLogic(OptionSetting);

			// 저장에 성공하면, 체크 박스에 표시해둠.
			if (bResult)
			{
				CrossHairOnCheckBox->SetIsChecked(OptionSetting.bIsCrossHairOn);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Error to save CrossHair settings"));
			}
		}
	}
}

// 조준점 설정 버튼 위에 마우스를 올려두면 작동할 함수.
void UGameSettingWidget::OnHoveredCrossHairButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 3으로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 3;
		UpdateButtonSlate();
		// CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredCrossHairButton()
//{
//	CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 마우스 민감도 버튼을 클릭하면 작동할 함수.
void UGameSettingWidget::OnClickMouseSensitiveButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 위젯의 모드를 마우스 민감도 설정 모드로 변경함.
		SetCurrentMode(CurrentType::MouseSensitiveSetting);
	}
}

// 마우스 민감도 버튼 위에 마우스를 올리면 작동할 함수.
void UGameSettingWidget::OnHoveredMouseSensitiveButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 4로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 4;
		UpdateButtonSlate();
		// MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredMouseSensitiveButton()
//{
//	MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 현재 위젯의 모드를 설정하는 함수.
void UGameSettingWidget::SetCurrentMode(CurrentType ModeType)
{
	// 현재 위젯 모드를 변경함.
	CurrentMode = ModeType;

	// 최초 모드인 경우 버튼의 개수는 6개(== 최대 인덱스는 5로) 설정
	if (ModeType == CurrentType::None)
	{
		MenuNumber = 6;
	}
	// 언어 설정 모드인 경우 버튼의 개수는 2개(== 최대 인덱스는 1로) 설정
	else if (ModeType == CurrentType::LanguageSetting)
	{
		MenuNumber = 2;
	}
	
	// 버튼의 상태를 업데이트 함.
	UpdateButtonSlate();
}

// 마스터 볼륨 설정 함수.
void UGameSettingWidget::OnChangeGlobalVolume(float inValue)
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 볼륨을 해당 값만큼 증가/감소 시킴.
	OptionSetting.Volume += inValue; 
	
	// 볼륨이 0에서 2 사이의 값만 가지도록 설정함
	OptionSetting.Volume = FMath::Clamp(OptionSetting.Volume, 0.f, 2.f);
	
	// 볼륨 바에서는 0에서 1 사이가 되도록 설정값에서 2를 나눠서 표시함.
	VolumeSettingBar->SetPercent(OptionSetting.Volume / 2.f);

	// 그 후, 마스터 볼륨을 실시간으로 변경함. 저장은 따로 하도록 설정함.
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, OptionSetting.Volume * 2);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
}

// 마우스 민감도를 변경하는 함수.
void UGameSettingWidget::OnChangeMouseSensitive(float inValue)
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 민감도를 해당 값만큼 증가/감소 시킴.
	OptionSetting.MouseSensitive += inValue;

	// 민감도가 0에서 5 사이의 값만 가지도록 설정함.
	OptionSetting.MouseSensitive = FMath::Clamp(OptionSetting.MouseSensitive, 0.f, 5.f);
	
	// 민감도 바에서는 0에서 1 사이가 되도록 설정값에서 5를 나눠 표시함.
	SensitiveSettingBar->SetPercent(OptionSetting.MouseSensitive / 5.f);

	// 저장은 따로 하도록 설정함.
}

// 뒤로 가기 버튼을 클릭했을 때 작동할 함수.
void UGameSettingWidget::OnClickBackButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 위젯 애니메이션을 재생함.
	PlayAnimation(BackOptionAnim);
}

// 뒤로 가기 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGameSettingWidget::OnHoveredBackButton()
{
	// 현재 아무 것도 선택되지 않은 상태일 때만 작동하도록 함.
	if (CurrentMode == CurrentType::None)
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}
		
		// 현재 버튼 인덱스를 5로 설정하고, 버튼의 상태를 업데이트함.
		MenuNavIndex = 5;
		UpdateButtonSlate();
		// BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

//void UGameSettingWidget::OnUnhoveredBackButton()
//{
//	BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 애니메이션이 끝났을 때 작동할 함수.
void UGameSettingWidget::AnimationFinished()
{
	// 게임 모드를 불러와 위젯을 바꿈.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

// 버튼의 상태를 업데이트할 함수.
void UGameSettingWidget::UpdateButtonSlate()
{
	// 현재 위젯 모드가 최초 모드인 경우
	if (CurrentMode == CurrentType::None)
	{
		switch (MenuNavIndex)
		{
			// 버튼 인덱스가 0일 때, 언어 설정 버튼만 흰색으로 설정함.
			case 0:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 버튼 인덱스가 1일 때, 볼륨 설정 버튼만 흰색으로 설정함.
			case 1:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 버튼 인덱스가 2일 때, 타이머 설정 버튼만 흰색으로 설정함.
			case 2:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 버튼 인덱스가 3일 때, 조준점 설정 버튼만 흰색으로 설정함.
			case 3:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 버튼 인덱스가 4일 때, 마우스 민감도 설정 버튼만 흰색으로 설정함.
			case 4:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 버튼 인덱스가 5일 때, 뒤로 가기 버튼만 흰색으로 설정함.
			case 5:
			{
				LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				return;
			}
		}
	}
	// 그 외 모드인 경우, 모든 버튼의 색을 회색 느낌이 들도록 설정함.
	else
	{
		LanguageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		VolumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		TimerButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		CrossHairButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		MouseSensitiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}

	// 현재 위젯 모드가 언어 설정 모드인 경우
	if (CurrentMode == CurrentType::LanguageSetting)
	{
		// 현재 언어가 영어로 설정되어 있으면
		if (OptionSetting.Language == ELanguage::LANG_En)
		{
			// 세부 버튼 인덱스가 0이면 영어 버튼을 완전한 빨간색으로 설정함
			if (SubMenuNavIndex == 0)
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			// 아니라면 영어 버튼을 살짝 투명한 빨간색으로 설정함
			else
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
		// 영어로 설정되어 있지 않다면
		else
		{
			// 세부 버튼 인덱스가 0이면 영어 버튼을 흰색으로 설정함
			if (SubMenuNavIndex == 0)
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
			// 아니라면 영어 버튼을 살짝 투명한 흰색으로 설정함.
			else
			{
				EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			}
		}
		
		// 현재 언어가 한국어로 설정되어 있으면
		if (OptionSetting.Language == ELanguage::LANG_Ko)
		{
			// 세부 버튼 인덱스가 1이면 한국어 버튼을 완전한 빨간색으로 설정함
			if (SubMenuNavIndex == 1)
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			}
			// 아니라면 한국어 버튼을 살짝 투명한 빨간색으로 설정함
			else
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			}
		}
		// 한국어가 아니라면
		else
		{
			// 세부 버튼 인덱스가 1이면 한국어 버튼을 흰색으로 설정함
			if (SubMenuNavIndex == 1)
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			}
			// 아니라면 한국어 버튼을 살짝 투명한 흰색으로 설정함.
			else
			{
				KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			}
		}
	}
	// 언어 설정 모드가 아닌 경우
	else
	{
		// 현재 영어로 설정되어 있으면, 영어 버튼만 붉게 설정함.
		if (OptionSetting.Language == ELanguage::LANG_En)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
		// 현재 한국어로 설정되어 있으면, 한국어 버튼만 붉게 설정함.
		else if (OptionSetting.Language == ELanguage::LANG_Ko)
		{
			EnglishButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
			KoreanButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UGameSettingWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		// 현재 위젯 모드가 최초 모드인 경우
		if (CurrentMode == CurrentType::None)
		{
			switch (MenuNavIndex)
			{
				// 현재 버튼 인덱스가 0이면 언어 설정 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickLanguageButton();
					break;
				}
				// 현재 버튼 인덱스가 1이면 볼륨 설정 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickVolumeButton();
					break;
				}
				// 현재 버튼 인덱스가 2이면 타이머 설정 버튼을 클릭한 효과를 줌.
				case 2:
				{
					OnClickTimerButton();
					break;
				}
				// 현재 버튼 인덱스가 3이면 조준점 설정 버튼을 클릭한 효과를 줌.
				case 3:
				{
					OnClickCrossHairButton();
					break;
				}
				// 현재 버튼 인덱스가 4이면 마우스 민감도 설정 버튼을 클릭한 효과를 줌.
				case 4:
				{
					OnClickMouseSensitiveButton();
					break;
				}
				// 현재 버튼 인덱스가 5이면 뒤로 가기 버튼을 클릭한 효과를 줌.
				case 5:
				{
					OnClickBackButton();
					break;
				}
			}
		}
		// 현재 위젯 모드가 언어 설정 모드라면
		else if (CurrentMode == CurrentType::LanguageSetting)
		{
			switch (SubMenuNavIndex)
			{
				// 현재 세부 버튼 인덱스가 0이면 영어 설정 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickEnglishButton();
					break;
				}
				// 현재 세부 버튼 인덱스가 1이면 한국어 설정 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickKoreanButton();
					break;
				}
			}
		}
		// 현재 위젯 모드가 볼륨 설정 모드라면
		else if (CurrentMode == CurrentType::VolumeSetting)
		{
			// 설정한 볼륨 값을 세이브 데이터에 영구히 저장함.
			if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				if (SoundMix != nullptr && SoundClass != nullptr)
				{
					UGameplayStatics::SetSoundMixClassOverride(GetWorld(), SoundMix, SoundClass, OptionSetting.Volume);
					UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);
					bool SaveResult = GameInstance->VolumeSaveLogic(OptionSetting.Volume);
				}
			}

			// 그 후 최초 모드로 돌아감.
			SetCurrentMode(CurrentType::None);
		}
		// 현재 위젯 모드가 마우스 민감도 설정 모드라면
		else if (CurrentMode == CurrentType::MouseSensitiveSetting)
		{
			// 설정한 마우스 민감도 값을 세이브 데이터에 영구히 저장함.
			if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
			{
				bool SaveResult = GameInstance->MouseSensitiveSaveLogic(OptionSetting.MouseSensitive);
			}

			// 그 후 최초 모드로 돌아감.
			SetCurrentMode(CurrentType::None);
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// 현재 위젯 모드가 최초 모드인 경우
		if (CurrentMode == CurrentType::None)
		{
			// S 키나 아래 화살표 키인 경우
			if (KeyType == "S" || KeyType == "Down")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 설정함.
				MenuNavIndex++;
				if (MenuNavIndex >= MenuNumber)
				{
					MenuNavIndex = 0;
				}
			}
			// W 키나 위 화살표 키인 경우
			else if (KeyType == "W" || KeyType == "Up")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				
				// 버튼 인덱스를 1 감소시키고, 0 미만으로 내려가면 최대 인덱스로 설정함.
				MenuNavIndex--;
				if (MenuNavIndex < 0)
				{
					MenuNavIndex = MenuNumber - 1;
				}
			}
			// A 키나 좌측 화살표 키인 경우
			else if (KeyType == "A" || KeyType == "Left")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 애니메이션을 재생함.
				PlayAnimation(BackOptionAnim);
			}
		}
		// 현재 위젯 모드가 언어 설정 모드인 경우
		else if (CurrentMode == CurrentType::LanguageSetting)
		{
			// D 키나 우측 화살표 키인 경우
			if (KeyType == "D" || KeyType == "Right")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 세부 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 설정함.
				SubMenuNavIndex++;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex = 0;
				}
			}
			// A 키나 좌측 화살표 키인 경우
			else if (KeyType == "A" || KeyType == "Left")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 세부 버튼 인덱스를 1 감소시키고, 0 미만으로 내려가면 최대 인덱스로 설정함.
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
		}
		// 현재 위젯 모드가 볼륨 설정 모드라면
		else if (CurrentMode == CurrentType::VolumeSetting)
		{
			// S 키, 아래 화살표 키, A 키, 또는 좌측 화살표 키인 경우, 볼륨 설정 값을 0.01 감소시키도록 함수를 호출함. 
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				OnChangeGlobalVolume(-0.01);
			}
			// W 키, 위 화살표 키, D 키, 또는 우측 화살표 키인 경우, 볼륨 설정 값을 0.01 증가시키도록 함수를 호출함.
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				OnChangeGlobalVolume(0.01);
			}
		}
		// 현재 위젯 모드가 마우스 민감도 설정 모드라면
		else if (CurrentMode == CurrentType::MouseSensitiveSetting)
		{
			// S 키, 아래 화살표 키, A 키, 또는 좌측 화살표 키인 경우, 민감도 설정 값을 0.05 감소시키도록 함수를 호출함.
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				OnChangeMouseSensitive(-0.05);
			}
			// W 키, 위 화살표 키, D 키, 또는 우측 화살표 키인 경우, 볼륨 설정 값을 0.05 증가시키도록 함수를 호출함.
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				OnChangeMouseSensitive(0.05);
			}
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}