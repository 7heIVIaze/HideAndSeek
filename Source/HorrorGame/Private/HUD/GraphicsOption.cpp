// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

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
#include "GameFramework/GameUserSettings.h"
#include "HUD/OptionWidget.h"
#include "Math/IntPoint.h"

void UGraphicsOption::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	ResolutionButton = Cast<UButton>(GetWidgetFromName(TEXT("Resolution")));
	WindowModeButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowMode")));
	BrightnessButton = Cast<UButton>(GetWidgetFromName(TEXT("BrightButton")));
	AntiAliasingButton = Cast<UButton>(GetWidgetFromName(TEXT("AntiAliasingBtn")));
	ShadowButton = Cast<UButton>(GetWidgetFromName(TEXT("ShadowBtn")));
	MotionBlurButton = Cast<UButton>(GetWidgetFromName(TEXT("MotionBlurBtn")));
	Resolution1Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution1Btn")));
	Resolution2Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution2Btn")));
	Resolution3Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution3Btn")));
	Resolution4Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution4Btn")));
	Resolution5Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution5Btn")));
	Resolution6Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution6Btn")));
	Resolution7Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution7Btn")));
	Resolution8Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution8Btn")));
	Resolution9Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution9Btn")));
	Resolution10Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution10Btn")));
	WindowedButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowBtn")));
	WinFullButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowFullBtn")));
	FullScreenButton = Cast<UButton>(GetWidgetFromName(TEXT("FullScreenBtn")));
	AntiAliasBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("AntiCheckbox")));
	ShadowBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("ShadowCheckbox")));
	MotionBlurBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("MotionBlurCheckbox")));
	BrightnessBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BrightBar")));

	MainBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("GraphicsSettingPanel")));
	ResolutionPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ResolutionType")));
	WindowModeBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("WindowModePanel")));

	if (IsValid(ResolutionButton))
	{
		ResolutionButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolutionButton);
		ResolutionButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolutionButton);
	}
	if (IsValid(WindowModeButton))
	{
		WindowModeButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWindowModeButton);
		WindowModeButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWindowModeButton);
	}
	if (IsValid(BrightnessButton))
	{
		BrightnessButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickBrightButton);
		BrightnessButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredBrightButton);
	}
	if (IsValid(AntiAliasingButton))
	{
		AntiAliasingButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickAntiAliasingButton);
		AntiAliasingButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredAntiAliasingButton);
	}
	if (IsValid(ShadowButton))
	{
		ShadowButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickShadowButton);
		ShadowButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredShadowButton);
	}
	if (IsValid(MotionBlurButton))
	{
		ResolutionButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickMotionBlurButton);
		ResolutionButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredMotionBlurButton);
	}
	if (IsValid(Resolution1Button))
	{
		Resolution1Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution1Button);
		Resolution1Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution1Button);
	}
	if (IsValid(Resolution2Button))
	{
		Resolution2Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution2Button);
		Resolution2Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution2Button);
	}
	if (IsValid(Resolution3Button))
	{
		Resolution3Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution3Button);
		Resolution3Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution3Button);
	}
	if (IsValid(Resolution4Button))
	{
		Resolution4Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution4Button);
		Resolution4Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution4Button);
	}
	if (IsValid(Resolution5Button))
	{
		Resolution5Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution5Button);
		Resolution5Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution5Button);
	}
	if (IsValid(Resolution6Button))
	{
		Resolution6Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution6Button);
		Resolution6Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution6Button);
	}
	if (IsValid(Resolution7Button))
	{
		Resolution7Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution7Button);
		Resolution7Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution7Button);
	}
	if (IsValid(Resolution8Button))
	{
		Resolution8Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution8Button);
		Resolution8Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution8Button);
	}
	if (IsValid(Resolution9Button))
	{
		Resolution9Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution9Button);
		Resolution9Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution9Button);
	}
	if (IsValid(Resolution10Button))
	{
		Resolution10Button->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution10Button);
		Resolution10Button->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution10Button);
	}
	if (IsValid(WindowedButton))
	{
		WindowedButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWindowedButton);
		WindowedButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWindowedButton);
	}
	if (IsValid(WinFullButton))
	{
		WinFullButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWinFullButton);
		WinFullButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWinFullButton);
	}
	if (IsValid(FullScreenButton))
	{
		FullScreenButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickFullScreenButton);
		FullScreenButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredFullScreenButton);
	}
	if (IsValid(MainBox))
	{
		MenuNumber = MainBox->GetChildrenCount();
	}
	
	UpdateButtonSlate();
	SetCurrentMode(GraphicsType::Option);
}

// Main Menu
void UGraphicsOption::OnClickResolutionButton()
{
	SubMenuNavIndex = 0;
	SetCurrentMode(GraphicsType::Resolution);
	if (IsValid(ResolutionPanel))
	{
		MenuNumber = ResolutionPanel->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredResolutionButton()
{
	MenuNavIndex = 0;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickWindowModeButton()
{
	SubMenuNavIndex = 0;
	SetCurrentMode(GraphicsType::Window);
	if (IsValid(WindowModeBox))
	{
		MenuNumber = WindowModeBox->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredWindowModeButton()
{
	MenuNavIndex = 1;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickBrightButton()
{
	SetCurrentMode(GraphicsType::Brightness);
	/*if (IsValid(WindowModeBox))
	{
		MenuNumber = WindowModeBox->GetChildrenCount();
	}*/
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredBrightButton()
{
	MenuNavIndex = 2;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickAntiAliasingButton()
{
	bIsAntiAliasing = !bIsAntiAliasing;
	AntiAliasBox->SetIsChecked(bIsAntiAliasing);
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredAntiAliasingButton()
{
	MenuNavIndex = 3;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickShadowButton()
{
	bShadowSetting = !bShadowSetting;
	ShadowBox->SetIsChecked(bShadowSetting);
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredShadowButton()
{
	MenuNavIndex = 4;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickMotionBlurButton()
{
	bMotionBlur = !bMotionBlur;
	MotionBlurBox->SetIsChecked(bMotionBlur);
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredMotionBlurButton()
{
	MenuNavIndex = 5;
	UpdateButtonSlate();
}

// Resolution Menu
void UGraphicsOption::OnClickResolution1Button()
{
	int32 x = 1024, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if(GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution1Button()
{
	SubMenuNavIndex = 0;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution2Button()
{
	int32 x = 1152, y = 864;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution2Button()
{
	SubMenuNavIndex = 1;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution3Button()
{
	int32 x = 1280, y = 720;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution3Button()
{
	SubMenuNavIndex = 2;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution4Button()
{
	int32 x = 1280, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution4Button()
{
	SubMenuNavIndex = 3;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution5Button()
{
	int32 x = 1280, y = 800;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution5Button()
{
	SubMenuNavIndex = 4;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution6Button()
{
	int32 x = 1280, y = 960;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution6Button()
{
	SubMenuNavIndex = 5;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution7Button()
{
	int32 x = 1280, y = 1024;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution7Button()
{
	SubMenuNavIndex = 6;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution8Button()
{
	int32 x = 1366, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution8Button()
{
	SubMenuNavIndex = 7;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution9Button()
{
	int32 x = 1600, y = 900;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting) 
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution9Button()
{
	SubMenuNavIndex = 8;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickResolution10Button()
{
	int32 x = 1920, y = 1080;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
	}
}

void UGraphicsOption::OnHoveredResolution10Button()
{
	SubMenuNavIndex = 9;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickWindowedButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if(UserSetting)
		UserSetting->SetFullscreenMode(EWindowMode::Windowed);
}

void UGraphicsOption::OnHoveredWindowedButton()
{
	SubMenuNavIndex = 0;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickWinFullButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
		UserSetting->SetFullscreenMode(EWindowMode::WindowedFullscreen);
}

void UGraphicsOption::OnHoveredWinFullButton()
{
	SubMenuNavIndex = 1;
	UpdateButtonSlate();
}

void UGraphicsOption::OnClickFullScreenButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
		UserSetting->SetFullscreenMode(EWindowMode::Fullscreen);
}

void UGraphicsOption::OnHoveredFullScreenButton()
{
	SubMenuNavIndex = 2;
	UpdateButtonSlate();
}

void UGraphicsOption::SetBrightness(int32 value)
{
	Brightness += value;

	BrightnessBar->SetPercent(0.5f);
}

void UGraphicsOption::UpdateButtonSlate()
{
	if (CurrentMode == GraphicsType::None)
	{
		switch (MenuNavIndex)
		{
		case 0:
			ResolutionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 2:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 3:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 4:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 5:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		default:
			ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		}
	}
	else
	{
		ResolutionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		WindowModeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BrightnessButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		AntiAliasingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ShadowButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		MotionBlurButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}
	
	if (CurrentMode == GraphicsType::Resolution)
	{
		switch (SubMenuNavIndex)
		{
		case 0:
			Resolution1Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 2:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 3:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 4:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 5:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 6:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 7:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 8:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 9:
			Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Resolution10Button->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
	}
	else
	{
		Resolution1Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution2Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution3Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution4Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution5Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution6Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution7Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution8Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution9Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Resolution10Button->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}
	
	if (CurrentMode == GraphicsType::Window)
	{
		switch (SubMenuNavIndex)
		{
		case 0:
			WindowedButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			WinFullButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			FullScreenButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			WindowedButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WinFullButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			FullScreenButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 2:
			WindowedButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WinFullButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			FullScreenButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
	}
	else
	{
		WindowedButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		WinFullButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		FullScreenButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}
}

void UGraphicsOption::SetCurrentMode(GraphicsType ModeType)
{
	CurrentMode = ModeType;
}

FReply UGraphicsOption::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	uint32 KeyNum = InKeyEvent.GetKeyCode();
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("KeyCode: %d"), KeyNum));*/
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (CurrentMode == GraphicsType::None)
	{
		if (KeyType == "Enter")
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickResolutionButton();
				break;
			case 1:
				OnClickWindowModeButton();
				break;
			case 2:
				OnClickBrightButton();
				break;
			case 3:
				OnClickAntiAliasingButton();
				break;
			case 4:
				OnClickShadowButton();
				break;
			case 5:
				OnClickMotionBlurButton();
				break;
			}
		}
		else
		{
			if (KeyType == "S" || KeyType == "Down")
			{
				MenuNavIndex++;
				if (MenuNavIndex >= MenuNumber)
				{
					MenuNavIndex = 0;
				}
			}
			else if (KeyType == "W" || KeyType == "Up")
			{
				MenuNavIndex--;
				if (MenuNavIndex < 0)
				{
					MenuNavIndex = MenuNumber - 1;
				}
			}
			UpdateButtonSlate();
		}
	}
	else if (CurrentMode == GraphicsType::Resolution)
	{
		if (KeyType == "Enter")
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickResolution1Button();
				break;
			case 1:
				OnClickResolution2Button();
				break;
			case 2:
				OnClickResolution3Button();
				break;
			case 3:
				OnClickResolution4Button();
				break;
			case 4:
				OnClickResolution5Button();
				break;
			case 5:
				OnClickResolution6Button();
				break;
			case 6:
				OnClickResolution7Button();
				break;
			case 7:
				OnClickResolution8Button();
				break;
			case 8:
				OnClickResolution9Button();
				break;
			case 9:
				OnClickResolution10Button();
				break;
			}
		}
		else
		{
			if (KeyType == "S" || KeyType == "Down")
			{
				SubMenuNavIndex++;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex = 0;
				}
			}
			else if (KeyType == "W" || KeyType == "Up")
			{
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
			UpdateButtonSlate();
		}
	}
	else if (CurrentMode == GraphicsType::Window)
	{
		if (KeyType == "Enter")
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickWindowedButton();
				break;
			case 1:
				OnClickWinFullButton();
				break;
			case 2:
				OnClickFullScreenButton();
				break;
			}
		}
		else
		{
			if (KeyType == "S" || KeyType == "Down")
			{
				SubMenuNavIndex++;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex = 0;
				}
			}
			else if (KeyType == "W" || KeyType == "Up")
			{
				SubMenuNavIndex--;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex = MenuNumber - 1;
				}
			}
			UpdateButtonSlate();
		}
	}
	else if (CurrentMode == GraphicsType::Brightness)
	{
		if (KeyType == "Enter")
		{
			SetCurrentMode(GraphicsType::None);
		}
		else
		{
			if (KeyType == "S" || KeyType == "Down" || KeyType == "D" || KeyType == "Right")
			{
				SetBrightness(1);
			}
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "A" || KeyType == "Left")
			{
				SetBrightness(-1);
			}
			UpdateButtonSlate();
		}
	}

	return reply;
}