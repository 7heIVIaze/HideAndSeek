// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/GraphicsOption.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "HorrorGameGameInstance.h"
#include "StartGameMode.h"
#include "StartWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Math/IntPoint.h"
#include "Engine/Console.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "Sound/SoundCue.h"
#include "Engine/PostProcessVolume.h"

// 그래픽 설정 위젯 생성자
void UGraphicsOption::NativeConstruct()
{
	Super::NativeConstruct();

	// 뷰포트 출력 시 애니메이션 효과를 출력함.
	PlayAnimation(NewOptionAnim);

	this->SetKeyboardFocus();

	// 뒤로가기 애니메이션이 끝나면, 뷰포트에서 제거되기 위해 델리게이트 함수를 바인딩해줌.
	EndDelegate.BindDynamic(this, &UGraphicsOption::AnimationFinished);
	BindToAnimationFinished(BackOptionAnim, EndDelegate);

	/*ResolutionButton = Cast<UButton>(GetWidgetFromName(TEXT("Resolution")));
	WindowModeButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowMode")));
	BrightnessBtn = Cast<UButton>(GetWidgetFromName(TEXT("BrightButton")));
	AntiAliasingBtn = Cast<UButton>(GetWidgetFromName(TEXT("AntiAliasingButton")));
	ShadowBtn = Cast<UButton>(GetWidgetFromName(TEXT("ShadowButton")));
	MotionBlurBtn = Cast<UButton>(GetWidgetFromName(TEXT("MotionBlurButton")));
	Resolution1Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution1Button")));
	Resolution2Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution2Button")));
	Resolution3Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution3Button")));
	Resolution4Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution4Button")));
	Resolution5Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution5Button")));
	Resolution6Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution6Button")));
	Resolution7Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution7Button")));
	Resolution8Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution8Button")));
	//Resolution9Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution9Button")));
	//Resolution10Btn = Cast<UButton>(GetWidgetFromName(TEXT("Resolution10Button")));
	WindowedBtn = Cast<UButton>(GetWidgetFromName(TEXT("WindowButton")));
	// WinFullBtn = Cast<UButton>(GetWidgetFromName(TEXT("WindowFullButton")));
	FullScreenBtn = Cast<UButton>(GetWidgetFromName(TEXT("FullScreenButton")));
	AntiAliasBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("AntiCheckbox")));
	ShadowBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("ShadowCheckbox")));
	MotionBlurBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("MotionBlurCheckbox")));
	BrightnessBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BrightBar")));

	MainBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("GraphicsSettingPanel")));
	ResolutionPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ResolutionType")));
	WindowModeBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("WindowModePanel")));*/

	// 각 버튼 별로 클릭과 마우스 호버 시 작동할 함수를 바인딩해줌.
	if (IsValid(ResolutionButton))
	{
		ResolutionButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolutionButton);
		ResolutionButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolutionButton);
		// Resolution->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolutionButton);
	}

	if (IsValid(ScreenSettingButton))
	{
		ScreenSettingButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickScreenSettingButton);
		ScreenSettingButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredScreenSettingButton);
		// WindowMode->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWindowModeButton);
	}

	if (IsValid(BrightButton))
	{
		BrightButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickBrightButton);
		BrightButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredBrightButton);
		// BrightButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredBrightButton);
	}

	if (IsValid(AntiAliasingButton))
	{
		AntiAliasingButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickAntiAliasingButton);
		AntiAliasingButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredAntiAliasingButton);
		// AntiAliasingButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredAntiAliasingButton);
	}

	if (IsValid(ShadowButton))
	{
		ShadowButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickShadowButton);
		ShadowButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredShadowButton);
		// ShadowButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredShadowButton);
	}

	if (IsValid(MotionBlurButton))
	{
		MotionBlurButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickMotionBlurButton);
		MotionBlurButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredMotionBlurButton);
		// MotionBlurButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredMotionBlurButton);
	}

	if (IsValid(Resolution1Button))
	{
		Resolution1Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution1Button);
		Resolution1Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution1Button);
		// Resolution1Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution1Button);
	}

	if (IsValid(Resolution2Button))
	{
		Resolution2Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution2Button);
		Resolution2Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution2Button);
		// Resolution2Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution2Button);
	}

	if (IsValid(Resolution3Button))
	{
		Resolution3Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution3Button);
		Resolution3Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution3Button);
		// Resolution3Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution3Button);
	}

	if (IsValid(Resolution4Button))
	{
		Resolution4Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution4Button);
		Resolution4Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution4Button);
		// Resolution4Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution4Button);
	}

	if (IsValid(Resolution5Button))
	{
		Resolution5Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution5Button);
		Resolution5Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution5Button);
		// Resolution5Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution5Button);
	}

	if (IsValid(Resolution6Button))
	{
		Resolution6Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution6Button);
		Resolution6Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution6Button);
		// Resolution6Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution6Button);
	}

	if (IsValid(Resolution7Button))
	{
		Resolution7Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution7Button);
		Resolution7Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution7Button);
		// Resolution7Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution7Button);
	}

	if (IsValid(Resolution8Button))
	{
		Resolution8Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution8Button);
		Resolution8Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution8Button);
		// Resolution8Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution8Button);
	}

	/*if (IsValid(Resolution9Button))
	{
		Resolution9Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution9Button);
		Resolution9Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution9Button);
		Resolution9Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution9Button);
	}
	if (IsValid(Resolution10Button))
	{
		Resolution10Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution10Button);
		Resolution10Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution10Button);
		Resolution10Button->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution10Button);
	}*/

	if (IsValid(WindowButton))
	{
		WindowButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWindowedButton);
		WindowButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWindowedButton);
		// WindowButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWindowedButton);
	}

	/*if (IsValid(WinFullButton))
	{
		WinFullButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWinFullButton);
		WinFullButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWinFullButton);
		WinFullButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWinFullButton);
	}*/

	if (IsValid(FullScreenButton))
	{
		FullScreenButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickFullScreenButton);
		FullScreenButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredFullScreenButton);
		// 	FullScreenButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredFullScreenButton);
	}

	if (BackButton != nullptr)
	{
		BackButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredBackButton);
		// BackButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredBackButton);
	}

	// 버튼의 개수는 그래픽 설정 패널의 버튼 개수로 설정.
	if (IsValid(GraphicsSettingPanel))
	{
		MenuNumber = GraphicsSettingPanel->GetChildrenCount();
	}

	// GameInstance로부터 밝기 감마 값과 모션 블러 설정값 가져오기(GameInstance는 실행 시 Save 데이터를 읽어옴)
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		Brightness = GameInstance->GetBrightGamma();
		bMotionBlur = GameInstance->GetMotionBlur();
	} 
	MotionBlurCheckbox->SetIsChecked(bMotionBlur);
	BrightBar->SetPercent(Brightness / 5.0f);
	
	// 유저 세팅 파일 가져오기
	if (GEngine)
	{
		UserSetting = GEngine->GetGameUserSettings(); 
	}

	// 안티 앨리어싱 효과 레벨이 3(Epic) 이상이면, 안티 앨리어싱이 적용된 것이고, 아니면 적용 안 된 것임.
	if (UserSetting->GetAntiAliasingQuality() == 3)
	{
		bIsAntiAliasing = true;
	}
	else
	{
		bIsAntiAliasing = false;
	}
	AntiCheckbox->SetIsChecked(bIsAntiAliasing);

	// 그림자 퀄리티가 3(Epic) 이상이면, 그림자 설정이 적용된 것이고, 아니면 적용 안 된 것.
	if (UserSetting->GetShadowQuality() == 3)
	{
		bShadowSetting = true;
	}
	else
	{
		bShadowSetting = false;
	}
	ShadowCheckbox->SetIsChecked(bShadowSetting);

	// 사용자 설정에 스크린 모드가 창모드인지, 전체화면 모드인지 체크
	if (UserSetting->GetFullscreenMode() == EWindowMode::Windowed)
	{
		bWindowed = true;
		bFullScreen = false;
	}
	else if (UserSetting->GetFullscreenMode() == EWindowMode::Fullscreen)
	{
		bWindowed = false;
		bFullScreen = true;
	}

	// 현재 화면 해상도 값도 가져옴.
	FIntPoint IntPoint = UserSetting->GetScreenResolution();
	CurrentResolution = FString::FromInt(IntPoint.X) + TEXT("x") + FString::FromInt(IntPoint.Y);
	CheckCurrentResolution(CurrentResolution);

	SetCurrentMode(GraphicsType::None);
	UpdateButtonSlate();
}

// Main Menu
// 해상도 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolutionButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 세부 버튼 인덱스를 0으로 설정하고, 현재 위젯 모드를 해상도 설정 모드로 변경함.
		SubMenuNavIndex = 0;
		SetCurrentMode(GraphicsType::Resolution);

		// 세부 버튼 최대 인덱스를 해상도 설정 버튼의 개수로 바꿈.
		if (IsValid(ResolutionType))
		{
			MenuNumber = ResolutionType->GetChildrenCount();
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 해상도 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolutionButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼의 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 0;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolutionButton()
//{
//	Resolution->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 화면 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickScreenSettingButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 세부 버튼 인덱스를 0으로 설정하고, 현재 위젯 모드를 스크린 설정 모드로 변경함.
		SubMenuNavIndex = 0;
		SetCurrentMode(GraphicsType::ScreenSetting);

		// 버튼의 최대 인덱스는 스크린 설정 패널의 버튼 개수(창모드/전체화면)(== 2)로 설정.
		if (IsValid(WindowModePanel))
		{
			MenuNumber = WindowModePanel->GetChildrenCount();
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 스크린 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredScreenSettingButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 1로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 1;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredWindowModeButton()
//{
//	WindowMode->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 밝기 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickBrightButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 위젯의 모드를 밝기 설정 모드로 설정하고, 버튼의 상태를 업데이트 함.
		SetCurrentMode(GraphicsType::Brightness);
		UpdateButtonSlate();
	}
}

// 밝기 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredBrightButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 2;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredBrightButton()
//{
//	BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 안티 앨리어싱 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickAntiAliasingButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		
		// 현재 설정된 안티 앨리어싱 값을 반전시켜 저장함.
		bIsAntiAliasing = !bIsAntiAliasing;
		AntiCheckbox->SetIsChecked(bIsAntiAliasing);

		// 해당 값을 유저 세팅 파일에 저장하는 로직.
		if (UserSetting)
		{ 
			// AntiAliasing이 true면 3(Epic), false면 0(low)으로 설정
			int32 Value = bIsAntiAliasing ? 3 : 0;

			// 그 후 설정된 값을 유저 세팅 파일에 저장하고 현재 게임에 적용함.
			UserSetting->SetAntiAliasingQuality(Value);
			UserSetting->ApplyNonResolutionSettings();
			UserSetting->SaveSettings();
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 안티앨리어싱 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredAntiAliasingButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 3으로 변경하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 3;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredAntiAliasingButton()
//{
//	AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 그림자 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickShadowButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 설정된 그림자 설정 값을 반전시켜 저장함.
		bShadowSetting = !bShadowSetting;
		ShadowCheckbox->SetIsChecked(bShadowSetting);

		// 해당 값을 유저 세팅 파일에 저장하는 로직.
		if (UserSetting)
		{
			// ShadowSetting이 true면 3(Epic), false면 0(Low)으로 설정
			int32 Value = bShadowSetting ? 3 : 0;

			// 그 후 설정된 값을 유저 세팅 파일에 저장하고, 현재 게임에 적용함.
			UserSetting->SetShadowQuality(Value);
			UserSetting->ApplyNonResolutionSettings();
			UserSetting->SaveSettings();
		}

		// 그 후 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 그림자 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredShadowButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 4로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 4;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredShadowButton()
//{
//	ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 모션블러 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickMotionBlurButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 현재 설정된 모션 블러값을 반전하여 저장함.
		bMotionBlur = !bMotionBlur;
		FString Command = bMotionBlur ? TEXT("r.DefaultFeature.MotionBlur 1") : TEXT("r.DefaultFeature.MotionBlur 0"); // true면 1, false면 0으로 설정

		// 게임 인스턴스 클래스를 가져옴.
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			// 설정한 값을 세이브 데이터에 영구히 저장함.
			bool bResult = GameInstance->MotionBlurSaveLogic(bMotionBlur);

			// 저장이 완료되면 게임에 적용하고, 모션 블러 체크 박스에 표시를 해 둠. 
			if (bResult)
			{
				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController)
				{
					PlayerController->ConsoleCommand(Command);
				}
				MotionBlurCheckbox->SetIsChecked(bMotionBlur);
				//GetWorld()->Exec(GetWorld(), *Command); // 저장을 완료해야 모션 블러 세팅 완료
			}
			// 저장 실패 시 로직임.
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Error to save MotionBlur settings"));
			}
		}

		// 그 후 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 모션 블러 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredMotionBlurButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 5로 설정하고, 버튼의 상태를 업데이트 함.
		MenuNavIndex = 5;
		UpdateButtonSlate();
	}
}

// deprecated
//void UGraphicsOption::OnUnhoveredMotionBlurButton()
//{
//	MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// Resolution Menu
// 1번 해상도(854x450) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution1Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 854x480임.
		int32 x = 854, y = 480;

		// 유저 설정 파일에 해상도를 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}
		/*bResolution1 = true;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution1;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 1번 해상도(854x480) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution1Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
	}
}

// deprecated
//void UGraphicsOption::OnUnhoveredResolution1Button()
//{
//	if (bResolution1)
//	{
//		Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 2번 해상도(1366x768) 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution2Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 1366x768임.
		int32 x = 1366, y = 768;

		// 유저 설정 파일에 해상도를 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}
		/*bResolution1 = false;
		bResolution2 = true;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution2;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 2번 해상도(1366x768) 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution2Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 1로 설정하고 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 1;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution2Button()
//{
//	if (bResolution2)
//	{
//		Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 3번 해상도(1600x900) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution3Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		
		// 화면 해상도는 1600x900임.
		int32 x = 1600, y = 900;

		// 유저 설정 파일에 해상도를 저장하고, 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}
		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = true;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution3;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 3번 해상도(1600x900) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution3Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 2;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution3Button()
//{
//	if (bResolution3)
//	{
//		Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 4번 해상도(1920x1080) 버튼을 클릭했을 때 적용할 함수.
void UGraphicsOption::OnClickResolution4Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 1920x1080임.
		int32 x = 1920, y = 1080;

		// 유저 설정 파일에 해상도를 저장하고, 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}

		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = true;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution4;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 4번 해상도(1920x1080) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution4Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 세부 버튼 인덱스를 3으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 3;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution4Button()
//{
//	if (bResolution4)
//	{
//		Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 5번 해상도(1280x800) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution5Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 1280x800임.
		int32 x = 1280, y = 800;

		// 유저 설정 파일에 해상도를 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}

		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = true;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution5;

		// 버튼의 상태를 업데이트함.
		UpdateButtonSlate();
	}
}

// 5번 해상도(1280x800) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution5Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 4로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 4;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution5Button()
//{
//	if (bResolution5)
//	{
//		Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 6번 해상도(1920x1200) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution6Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 1920x1200임.
		int32 x = 1920, y = 1200;

		// 유저 설정 파일에 해상도를 저장하고, 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}

		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = true;
		bResolution7 = false;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution6;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 6번 해상도(1920x1200) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution6Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 5로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 5;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution6Button()
//{
//	if (bResolution6)
//	{
//		Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 7번 해상도(2560x1080) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution7Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 2560x1080임.
		int32 x = 2560, y = 1080;

		// 유저 세팅 파일에 해상도를 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}

		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = true;
		bResolution8 = false;*/

		ResolutionValue = EResolution::Resolution7;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 7번 해상도(2560x1080) 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution7Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 6으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 6;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution7Button()
//{
//	if (bResolution7)
//	{
//		Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 8번 해상도(3440x1440) 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickResolution8Button()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 화면 해상도는 3440x1440임.
		int32 x = 3440, y = 1440;

		// 유저 설정 파일에 해상도를 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetScreenResolution(FIntPoint(x, y));
			UserSetting->ApplyResolutionSettings(true);
			UserSetting->SaveSettings();
		}

		/*bResolution1 = false;
		bResolution2 = false;
		bResolution3 = false;
		bResolution4 = false;
		bResolution5 = false;
		bResolution6 = false;
		bResolution7 = false;
		bResolution8 = true;*/

		ResolutionValue = EResolution::Resolution8;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 8번 해상도(3440x1440) 버튼 위에 마우스를 올릴 때 작동할 함수.
void UGraphicsOption::OnHoveredResolution8Button()
{
	// 현재 위젯의 모드가 해상도 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 7로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 7;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredResolution8Button()
//{
//	if (bResolution8)
//	{
//		Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// DEPRECATED
//void UGraphicsOption::OnClickResolution9Button()
//{
//	int32 x = 1600, y = 900;
//	UGameUserSettings* UserSetting = nullptr;
//	if (GEngine)
//		UserSetting = GEngine->GetGameUserSettings();
//	if (UserSetting) 
//	{
//		UserSetting->SetScreenResolution(FIntPoint(x, y));
//		UserSetting->ApplyResolutionSettings(true);
//	}
//}
//
//void UGraphicsOption::OnHoveredResolution9Button()
//{
//	if (CurrentMode == GraphicsType::Resolution)
//	{
//		SubMenuNavIndex = 8;
//		UpdateButtonSlate();
//	}
//}
//
//void UGraphicsOption::OnUnhoveredResolution9Button()
//{
//	if (bResolution9)
//	{
//		Resolution9Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution9Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}
//
//void UGraphicsOption::OnClickResolution10Button()
//{
//	int32 x = 1920, y = 1080;
//	UGameUserSettings* UserSetting = nullptr;
//	if (GEngine)
//		UserSetting = GEngine->GetGameUserSettings();
//	if (UserSetting)
//	{
//		UserSetting->SetScreenResolution(FIntPoint(x, y));
//		UserSetting->ApplyResolutionSettings(true);
//	}
//}
//
//void UGraphicsOption::OnHoveredResolution10Button()
//{
//	if (CurrentMode == GraphicsType::Resolution)
//	{
//		SubMenuNavIndex = 9;
//		UpdateButtonSlate();
//	}
//}
//
//void UGraphicsOption::OnUnhoveredResolution10Button()
//{
//	if (bResolution10)
//	{
//		Resolution10Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution10Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 창모드 설정 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickWindowedButton()
{
	// 현재 위젯의 모드가 화면 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::ScreenSetting)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 유저 설정 파일에 창모드 설정을 저장하고 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetFullscreenMode(EWindowMode::Windowed);
			UserSetting->ApplySettings(true);
		}
		bWindowed = true;
		bFullScreen = false;

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 창모드 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredWindowedButton()
{
	// 현재 위젯의 모드가 화면 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::ScreenSetting)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 세부 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredWindowedButton()
//{
//	if (bWindowed)
//	{
//		WindowButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		WindowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// DEPRECATED
//void UGraphicsOption::OnClickWinFullButton()
//{
//	UGameUserSettings* UserSetting = nullptr;
//	if (GEngine)
//		UserSetting = GEngine->GetGameUserSettings();
//	if (UserSetting)
//		UserSetting->SetFullscreenMode(EWindowMode::WindowedFullscreen);
//}
//
//void UGraphicsOption::OnHoveredWinFullButton()
//{
//	if (CurrentMode == GraphicsType::Window)
//	{
//		SubMenuNavIndex = 1;
//		UpdateButtonSlate();
//	}
//}
//
//void UGraphicsOption::OnUnhoveredWinFullButton()
//{
//	if (bWinFull)
//	{
//		WinFullButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		WinFullButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 전체 화면 버튼을 클릭했을 때 작동할 함수.
void UGraphicsOption::OnClickFullScreenButton()
{
	// 현재 위젯의 모드가 화면 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::ScreenSetting)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 유저 설정 파일에 전체화면 모드를 저장하고, 게임에 적용함.
		if (UserSetting)
		{
			UserSetting->SetFullscreenMode(EWindowMode::Fullscreen);
			UserSetting->ApplySettings(true);
			UserSetting->SaveSettings();
		}

		bWindowed = false;
		bFullScreen = true;
		
		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}
}

// 전체화면 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredFullScreenButton()
{
	// 현재 위젯의 모드가 화면 설정 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::ScreenSetting)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 세부 버튼 인덱스를 1로 설정하고, 버튼의 상태를 업데이트 함.
		SubMenuNavIndex = 1;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredFullScreenButton()
//{
//	if (bFullScreen)
//	{
//		FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

// 밝기를 설정하는 함수.
void UGraphicsOption::SetBrightness(float Value)
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 밝기를 0.1만큼 더하거나 뺌.
	Brightness += Value;
	
	// 밝기는 0에서 5 사이의 값을 가지도록 범위를 제한함.
	Brightness = FMath::Clamp(Brightness, 0.f, 5.f);

	// 밝기 바는 0에서 1 사이의 값을 가지기 때문에 5로 나눔.
	BrightBar->SetPercent(Brightness / 5.f);

	// 밝기 설정 커맨드
	FString CVarGamma = TEXT("gamma ") + FString::Printf(TEXT("%f"), Brightness);

	// 게임 인스턴스 클래스를 가져옴.
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		// 설정한 밝기 값을 세이브 데이터에 영구히 저장함.
		bool Result = GameInstance->BrightGammaSaveLogic(Brightness);
		
		// 저장에 성공하면, 게임에 적용함.
		if (Result)
		{
			// UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass());
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				PlayerController->ConsoleCommand(CVarGamma);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No Player Controller!"));
			}
		}
		// 저장에 실패할 경우 로직
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error to save Brightness settings"));
		}
	}

}

// 뒤로 가기 버튼을 클릭할 때 작동할 함수.
void UGraphicsOption::OnClickBackButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 애니메이션을 재생함.
		PlayAnimation(BackOptionAnim);
	}
}

// 뒤로 가기 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UGraphicsOption::OnHoveredBackButton()
{
	// 현재 위젯의 모드가 최초 모드인 경우에만 작동하도록 설정함.
	if (CurrentMode == GraphicsType::None)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 6으로 설정하고 버튼의 상태를 업데이트 함.
		MenuNavIndex = 6;
		UpdateButtonSlate();
	}
}

// Deprecated
//void UGraphicsOption::OnUnhoveredBackButton()
//{
//	BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//}

// 버튼의 상태를 업데이트하는 함수.
void UGraphicsOption::UpdateButtonSlate()
{
	// 현재 위젯의 모드가 최초 모드인 경우
	if (CurrentMode == GraphicsType::None)
	{
		switch (MenuNavIndex)
		{
			// 현재 버튼 인덱스가 0이면 해상도 설정 버튼만 흰색으로 설정함.
			case 0:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 1이면 화면 설정 버튼만 흰색으로 설정함.
			case 1:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 2이면 밝기 설정 버튼만 흰색으로 설정함.
			case 2:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 3이면 안티 앨리어싱 설정 버튼만 흰색으로 설정함.
			case 3:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 4이면 그림자 설정 버튼만 흰색으로 설정함.
			case 4:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 5이면 모션 블러 설정 버튼만 흰색으로 설정함.
			case 5:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				return;
			}
			// 현재 버튼 인덱스가 6이면 뒤로 가기 버튼만 흰색으로 설정함.
			case 6:
			{
				ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
				BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				return;
			}
		}
	}
	// 최초 모드가 아닐 경우, 모든 버튼의 색을 회색으로 설정함.
	else
	{
		ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		ScreenSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
	
	// 현재 위젯의 모드가 해상도 설정 모드인 경우
	if (CurrentMode == GraphicsType::Resolution)
	{
		// 해당 함수에서 버튼을 업데이트 함.
		SelectResolutionMode(ResolutionValue, SubMenuNavIndex);
	}
	// 해상도 설정 모드가 아닐 경우
	else
	{
		// 해당 함수에서 버튼을 업데이트 하되, 인덱스가 -1인 상태로 업데이트 함.
		SelectResolutionMode(ResolutionValue, -1);
	}
	
	// 현재 위젯의 모드가 화면 설정 모드인 경우
	if (CurrentMode == GraphicsType::ScreenSetting)
	{
		// 해당 함수에서 버튼을 업데이트 함.
		SelectWindowMode(bWindowed, bFullScreen, SubMenuNavIndex);
	}
	// 화면 설정 모드가 아닌 경우
	else
	{
		// 해당 함수에서 버튼을 업데이트 하되, 인덱스가 -1인 상태로 업데이트 함.
		SelectWindowMode(bWindowed, bFullScreen, -1);
	}
}

// 창모드 설정 값에 따라 버튼의 상태를 업데이트하는 함수
void UGraphicsOption::SelectWindowMode(bool bWindowedMode, bool bFullScreenMode, int32 CurrentIndex)
{
	// 창모드 상태인 경우
	if (bWindowedMode)
	{
		// 현재 세부 버튼 인덱스가 0일 때 창모드 버튼만 붉게 설정함.
		if (CurrentIndex == 0)
		{
			WindowButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 세부 버튼 인덱스가 0이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			WindowButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 창모드 상태가 아닌 경우
	else
	{
		// 현재 세부 버튼 인덱스가 0일 때 창모드 버튼만 흰색으로 설정함.
		if (CurrentIndex == 0)
		{
			WindowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 세부 버튼 인덱스가 0이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			WindowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 전체화면 상태인 경우
	if (bFullScreenMode)
	{
		// 세부 버튼 인덱스가 1일 때, 전체화면 버튼만 붉지만 희미하게 설정함.
		if (CurrentIndex == 1)
		{
			FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 세부 버튼 인덱스가 1이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 전체화면 상태가 아닌 경우
	else
	{
		// 세부 버튼 인덱스가 1이 아닐 때, 전체화면 버튼만 흰색으로 설정함.
		if (CurrentIndex == 1)
		{
			FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 세부 버튼 인덱스가 1이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}
}

//void UGraphicsOption::SelectResolutionMode(bool bResolution1Mode, bool bResolution2Mode, bool bResolution3Mode, bool bResolution4Mode,
//	bool bResolution5Mode, bool bResolution6Mode, bool bResolution7Mode, bool bResolution8Mode, int32 CurrentIndex)
// 해상도 설정 값에 따라 버튼의 상태를 업데이트하는 함수.
void UGraphicsOption::SelectResolutionMode(EResolution inResolutionValue, int32 CurrentIndex)
{
	// 현재 해상도 설정 값이 1번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution1)
	{
		// 현재 세부 버튼 인덱스가 0일 때, 1번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 0)
		{
			Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 0이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 1번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 0일 때, 1번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 0)
		{
			Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 0이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 2번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution2)
	{
		// 현재 세부 버튼 인덱스가 1일 때, 2번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 1)
		{
			Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 1이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 2번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 1일 때, 2번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 1)
		{
			Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 1이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 3번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution3)
	{
		// 현재 세부 버튼 인덱스가 2일 때, 3번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 2)
		{
			Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 2가 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 3번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 2일 때, 3번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 2)
		{
			Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 2가 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 4번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution4)
	{
		// 현재 세부 버튼 인덱스가 3일 때, 4번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 3)
		{
			Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 3이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 4번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 3일 때, 4번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 3)
		{
			Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 3이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 5번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution5)
	{
		// 현재 세부 버튼 인덱스가 4일 때, 5번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 4)
		{
			Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 4가 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 5번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 4일 때, 5번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 4)
		{
			Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 4가 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 6번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution6)
	{
		// 현재 세부 버튼 인덱스가 5일 때, 6번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 5)
		{
			Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 5가 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 6번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 5일 때, 6번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 5)
		{
			Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 5가 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 7번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution7)
	{
		// 현재 세부 버튼 인덱스가 6일 때, 7번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 6)
		{
			Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 6이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 7번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 6일 때, 7번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 6)
		{
			Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 6이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	// 현재 해상도 설정 값이 8번 해상도 값이면
	if (inResolutionValue == EResolution::Resolution8)
	{
		// 현재 세부 버튼 인덱스가 7일 때, 8번 해상도 버튼만 붉은 색으로 설정함.
		if (CurrentIndex == 7)
		{
			Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 7이 아닐 때, 붉지만 희미하게 설정함.
		else
		{
			Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	// 해상도 설정 값이 8번 해상도 값이 아니면
	else
	{
		// 현재 세부 버튼 인덱스가 7일 때, 8번 해상도 버튼만 흰색으로 설정함.
		if (CurrentIndex == 7)
		{
			Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		// 현재 세부 버튼 인덱스가 7이 아닐 때, 흰색이지만 희미하게 설정함.
		else
		{
			Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}
}

// 현재 위젯의 모드를 설정함.
void UGraphicsOption::SetCurrentMode(GraphicsType ModeType)
{
	// 현재 모드를 설정함.
	CurrentMode = ModeType;

	// 최초 모드일 경우 버튼 최대 인덱스는 그래픽스 설정 패널의 버튼 수로 설정함.
	if (ModeType == GraphicsType::None)
	{
		MenuNumber = GraphicsSettingPanel->GetChildrenCount();
	}
	// 해상도 설정 모드일 경우 버튼 최대 인덱스는 해상도 설정 패널의 버튼 수로 설정함.
	else if (ModeType == GraphicsType::Resolution)
	{
		MenuNumber = ResolutionType->GetChildrenCount();
	}
	// 화면 설정 모드일 경우 버튼 최대 인덱스는 화면 설정 패널의 버튼 수로 설정함.
	else if (ModeType == GraphicsType::ScreenSetting)
	{
		MenuNumber = WindowModePanel->GetChildrenCount();
	}
}

// 현재 해상도를 확인하는 함수.
void UGraphicsOption::CheckCurrentResolution(const FString ForCheckResolution)
{
	/*bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;*/

	// 해당하는 해상도 값에 따라 현재 해상도 값을 설정해줌.
	if (ForCheckResolution.Equals(TEXT("854x480")))
	{
		ResolutionValue = EResolution::Resolution1;
	}
	else if (ForCheckResolution.Equals(TEXT("1366x768")))
	{
		ResolutionValue = EResolution::Resolution2;
	}
	else if (ForCheckResolution.Equals(TEXT("1600x900")))
	{
		ResolutionValue = EResolution::Resolution3;
	}
	else if (ForCheckResolution.Equals(TEXT("1920x1080")))
	{
		ResolutionValue = EResolution::Resolution4;
	}
	else if (ForCheckResolution.Equals(TEXT("1280x800")))
	{
		ResolutionValue = EResolution::Resolution5;
	}
	else if (ForCheckResolution.Equals(TEXT("1920x1200")))
	{
		ResolutionValue = EResolution::Resolution6;
	}
	else if (ForCheckResolution.Equals(TEXT("2560x1080")))
	{
		ResolutionValue = EResolution::Resolution7;
	}
	else if (ForCheckResolution.Equals(TEXT("3440x1440")))
	{
		ResolutionValue = EResolution::Resolution8;
	}
}

// 애니메이션이 끝났을 때 작동할 함수.
void UGraphicsOption::AnimationFinished()
{
	// 게임 모드를 가져와서 옵션 설정 위젯을 뷰포트에 출력하고 이 위젯을 제거함.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UGraphicsOption::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	uint32 KeyNum = InKeyEvent.GetKeyCode();
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		// 현재 위젯 모드가 최초 모드인 경우
		if (CurrentMode == GraphicsType::None)
		{
			switch (MenuNavIndex)
			{
				// 현재 버튼 인덱스가 0일 때 해상도 설정 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickResolutionButton();
					break;
				}
				// 현재 버튼 인덱스가 1일 때 화면 설정 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickScreenSettingButton();
					break;
				}
				// 현재 버튼 인덱스가 2일 때 밝기 설정 버튼을 클릭한 효과를 줌.
				case 2:
				{
					OnClickBrightButton();
					break;
				}
				// 현재 버튼 인덱스가 3일 때 안티 앨리어싱 설정 버튼을 클릭한 효과를 줌.
				case 3:
				{
					OnClickAntiAliasingButton();
					break;
				}
				// 현재 버튼 인덱스가 4일 때 그림자 설정 버튼을 클릭한 효과를 줌.
				case 4:
				{
					OnClickShadowButton();
					break;
				}
				// 현재 버튼 인덱스가 5일 때 모션 블러 설정 버튼을 클릭한 효과를 줌.
				case 5:
				{
					OnClickMotionBlurButton();
					break;
				}
				// 현재 버튼 인덱스가 6일 때 뒤로 가기 버튼을 클릭한 효과를 줌.
				case 6:
				{
					OnClickBackButton();
					break;
				}
			}
		}
		// 현재 위젯 모드가 해상도 설정 모드인 경우
		else if (CurrentMode == GraphicsType::Resolution)
		{
			switch (SubMenuNavIndex)
			{
				// 현재 세부 버튼 인덱스가 0일 때 1번 해상도 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickResolution1Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 1일 때 2번 해상도 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickResolution2Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 2일 때 3번 해상도 버튼을 클릭한 효과를 줌.
				case 2:
				{
					OnClickResolution3Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 3일 때 4번 해상도 버튼을 클릭한 효과를 줌.
				case 3:
				{
					OnClickResolution4Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 4일 때 5번 해상도 버튼을 클릭한 효과를 줌.
				case 4:
				{
					OnClickResolution5Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 5일 때 6번 해상도 버튼을 클릭한 효과를 줌.
				case 5:
				{
					OnClickResolution6Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 6일 때 7번 해상도 버튼을 클릭한 효과를 줌.
				case 6:
				{
					OnClickResolution7Button();
					break;
				}
				// 현재 세부 버튼 인덱스가 7일 때 8번 해상도 버튼을 클릭한 효과를 줌.
				case 7:
				{
					OnClickResolution8Button();
					break;
				}
				/*case 8:
					OnClickResolution9Button();
					break;
				case 9:
					OnClickResolution10Button();
					break;*/
			}

			// 그 후 위젯의 모드를 최초 모드로 변경하고 버튼의 상태를 업데이트 함.
			SetCurrentMode(GraphicsType::None);
			UpdateButtonSlate();
		}
		// 현재 위젯의 모드가 화면 설정 모드인 경우
		else if (CurrentMode == GraphicsType::ScreenSetting)
		{
			switch (SubMenuNavIndex)
			{
				// 현재 세부 버튼 인덱스가 0일 때 창모드 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickWindowedButton();
					break;
				}
				// 현재 세부 버튼 인덱스가 1일 때 전체화면 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickFullScreenButton();
					break;
				}
			}

			// 그 후 위젯의 모드를 최초 모드로 변경하고 버튼의 상태를 업데이트 함.
			SetCurrentMode(GraphicsType::None);
			UpdateButtonSlate();
		}
		// 현재 위젯의 모드가 밝기 설정 모드인 경우
		else if (CurrentMode == GraphicsType::Brightness)
		{
			// 그 후 위젯의 모드를 최초 모드로 변경하고 버튼의 상태를 업데이트 함.
			SetCurrentMode(GraphicsType::None);
			UpdateButtonSlate();
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// 현재 위젯의 모드가 최조 모드인 경우
		if (CurrentMode == GraphicsType::None)
		{
			// S 키이거나 아래 화살표 키인 경우
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
			// W 키이거나 위 화살표 키인 경우
			else if (KeyType == "W" || KeyType == "Up")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 버튼 인덱스를 1 감소시키고, 0 미만일 경우, 최대 인덱스로 설정함.
				MenuNavIndex--;
				if (MenuNavIndex < 0)
				{
					MenuNavIndex = MenuNumber - 1;
				}
			}
			// A 키이거나 좌측 화살표 키인 경우
			else if (KeyType == "A" || KeyType == "Left")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 뒤로 가기 애니메이션을 출력함.
				PlayAnimation(BackOptionAnim);
			}
		}
		// 현재 위젯의 모드가 해상도 설정 모드인 경우
		else if (CurrentMode == GraphicsType::Resolution)
		{
			// S 키이거나 아래 화살표 키인 경우
			if (KeyType == "S" || KeyType == "Down")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 세부 버튼 인덱스를 4 증가시키고, 최대 인덱스를 넘어가면 8만큼 빼 줌.
				SubMenuNavIndex += 4;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex -= 8;
				}
			}
			// W 키이거나 위 화살표 키인 경우
			else if (KeyType == "W" || KeyType == "Up")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 세부 버튼 인덱스를 4 감소시키고, 0 미만일 경우, 8만큼 더해 줌.
				SubMenuNavIndex -= 4;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex += 8;
				}
			}
			// A 키이거나 좌측 화살표 키인 경우
			else if (KeyType == "A" || KeyType == "Left")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				
				// 세부 버튼 인덱스를 1 감소시키고, 0 미만일 경우, 최대 인덱스로 설정함.
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
			// D 키이거나 우측 화살표 키인 경우
			else if (KeyType == "D" || KeyType == "Right")
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
		}
		// 현재 위젯의 모드가 화면 설정 모드인 경우
		else if (CurrentMode == GraphicsType::ScreenSetting)
		{
			// D 키이거나 우측 화살표 키인 경우
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
			// A 키이거나 좌측 화살표 키인 경우
			else if (KeyType == "A" || KeyType == "Left")
			{
				// 버튼 이동 음을 재생함.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				// 세부 버튼 인덱스를 1 감소시키고, 0 미만일 경우, 최대 인덱스로 설정함.
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
		}
		// 현재 위젯의 모드가 밝기 설정 모드인 경우
		else if (CurrentMode == GraphicsType::Brightness)
		{
			// S 키, 아래 화살표 키, A 키, 좌측 화살표 키인 경우, 밝기 값을 0.1 감소시킴.
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				SetBrightness(-0.1);
			}
			// W 키, 위 화살표 키, D 키, 우측 화살표 키인 경우, 밝기 값을 0.1 증가시킴.
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				SetBrightness(0.1);
			}
		}

		// 그 후 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}