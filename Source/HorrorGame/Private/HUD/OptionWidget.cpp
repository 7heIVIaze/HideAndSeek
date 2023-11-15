// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/OptionWidget.h"
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

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	PlaySettingButton = Cast<UButton>(GetWidgetFromName(TEXT("PlayOptionBtn")));
	GraphicsSettingButton = Cast<UButton>(GetWidgetFromName(TEXT("GraphicOptionBtn")));
	KeyMappingButton = Cast<UButton>(GetWidgetFromName(TEXT("KeyMappingBtn")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackToMainBtn")));
	MenuBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("OptionSelectBox")));
	MenuLine = Cast<UBorder>(GetWidgetFromName(TEXT("Line")));

	if (nullptr != PlaySettingButton)
	{
		PlaySettingButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickPlaySettingButton);
		PlaySettingButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredPlaySettingButton);
	}

	if (nullptr != GraphicsSettingButton)
	{
		GraphicsSettingButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickGraphicsSettingButton);
		GraphicsSettingButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredGraphicsSettingButton);
	}

	if (nullptr != KeyMappingButton)
	{
		KeyMappingButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickKeyMappingButton);
		KeyMappingButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredKeyMappingButton);
	}

	if (nullptr != BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredBackButton);
	}

	if (nullptr != MenuBox)
	{
		MenuNumber = MenuBox->GetChildrenCount();
	}

	// Graphics Option
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
		ResolutionButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolutionButton);
		ResolutionButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolutionButton);
	}
	if (IsValid(WindowModeButton))
	{
		WindowModeButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickWindowModeButton);
		WindowModeButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredWindowModeButton);
	}
	if (IsValid(BrightnessButton))
	{
		BrightnessButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickBrightButton);
		BrightnessButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredBrightButton);
	}
	if (IsValid(AntiAliasingButton))
	{
		AntiAliasingButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickAntiAliasingButton);
		AntiAliasingButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredAntiAliasingButton);
	}
	if (IsValid(ShadowButton))
	{
		ShadowButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickShadowButton);
		ShadowButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredShadowButton);
	}
	if (IsValid(MotionBlurButton))
	{
		ResolutionButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickMotionBlurButton);
		ResolutionButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredMotionBlurButton);
	}
	if (IsValid(Resolution1Button))
	{
		Resolution1Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution1Button);
		Resolution1Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution1Button);
	}
	if (IsValid(Resolution2Button))
	{
		Resolution2Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution2Button);
		Resolution2Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution2Button);
	}
	if (IsValid(Resolution3Button))
	{
		Resolution3Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution3Button);
		Resolution3Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution3Button);
	}
	if (IsValid(Resolution4Button))
	{
		Resolution4Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution4Button);
		Resolution4Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution4Button);
	}
	if (IsValid(Resolution5Button))
	{
		Resolution5Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution5Button);
		Resolution5Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution5Button);
	}
	if (IsValid(Resolution6Button))
	{
		Resolution6Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution6Button);
		Resolution6Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution6Button);
	}
	if (IsValid(Resolution7Button))
	{
		Resolution7Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution7Button);
		Resolution7Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution7Button);
	}
	if (IsValid(Resolution8Button))
	{
		Resolution8Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution8Button);
		Resolution8Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution8Button);
	}
	if (IsValid(Resolution9Button))
	{
		Resolution9Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution9Button);
		Resolution9Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution9Button);
	}
	if (IsValid(Resolution10Button))
	{
		Resolution10Button->OnClicked.AddDynamic(this, &UOptionWidget::OnClickResolution10Button);
		Resolution10Button->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredResolution10Button);
	}
	if (IsValid(WindowedButton))
	{
		WindowedButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickWindowedButton);
		WindowedButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredWindowedButton);
	}
	if (IsValid(WinFullButton))
	{
		WinFullButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickWinFullButton);
		WinFullButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredWinFullButton);
	}
	if (IsValid(FullScreenButton))
	{
		FullScreenButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickFullScreenButton);
		FullScreenButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredFullScreenButton);
	}
	if (IsValid(MainBox))
	{
		MenuNumber = MainBox->GetChildrenCount();
	}

	bIsAntiAliasing = AntiAliasBox->IsChecked();
	bShadowSetting = ShadowBox->IsChecked();
	bMotionBlur = MotionBlurBox->IsChecked();
	UpdateButtonSlate();
	SetCurrentMode(OptionType::None);
	SetCurrentGraphicsMode(GraphicsType::Option);
}

void UOptionWidget::OnClickPlaySettingButton()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("PlaySetting Click"));
	// PlayAnimation(NewOptionAnim);
	// SetCurrentMode(OptionType::PlayerSetting);
}

void UOptionWidget::OnHoveredPlaySettingButton()
{
	MenuNavIndex = 0;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickGraphicsSettingButton()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("GraphicsSetting Click"));*/
	GraphicMenuNavIndex = 0;
	PlayAnimation(NewOptionAnim);
	MenuNumber = MainBox->GetChildrenCount();
	SetCurrentMode(OptionType::GraphicSetting);
	SetCurrentGraphicsMode(GraphicsType::None);
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredGraphicsSettingButton()
{
	MenuNavIndex = 1;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickKeyMappingButton()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("KeyMapping Click"));
	//PlayAnimation(NewOptionAnim);
	// SetCurrentMode(OptionType::KeySetting);
}

void UOptionWidget::OnHoveredKeyMappingButton()
{
	MenuNavIndex = 2;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickBackButton()
{
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::StartWidget);
}

void UOptionWidget::OnHoveredBackButton()
{
	MenuNavIndex = 3;
	UpdateButtonSlate();
}

// Main Menu
void UOptionWidget::OnClickResolutionButton()
{
	MenuNumber = ResolutionPanel->GetChildrenCount();
	SubMenuNavIndex = 0;
	SetCurrentGraphicsMode(GraphicsType::Resolution);
	if (IsValid(ResolutionPanel))
	{
		MenuNumber = ResolutionPanel->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredResolutionButton()
{
	GraphicMenuNavIndex = 0;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickWindowModeButton()
{
	MenuNumber = WindowModeBox->GetChildrenCount();
	SubMenuNavIndex = 0;
	SetCurrentGraphicsMode(GraphicsType::Window);
	if (IsValid(WindowModeBox))
	{
		MenuNumber = WindowModeBox->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredWindowModeButton()
{
	GraphicMenuNavIndex = 1;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickBrightButton()
{
	SetCurrentGraphicsMode(GraphicsType::Brightness);
	/*if (IsValid(WindowModeBox))
	{
		MenuNumber = WindowModeBox->GetChildrenCount();
	}*/
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredBrightButton()
{
	GraphicMenuNavIndex = 2;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickAntiAliasingButton()
{
	bIsAntiAliasing = !bIsAntiAliasing;
	AntiAliasBox->SetIsChecked(bIsAntiAliasing);
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredAntiAliasingButton()
{
	GraphicMenuNavIndex = 3;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickShadowButton()
{
	bShadowSetting = !bShadowSetting;
	ShadowBox->SetIsChecked(bShadowSetting);
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredShadowButton()
{
	GraphicMenuNavIndex = 4;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickMotionBlurButton()
{
	bMotionBlur = !bMotionBlur;
	MotionBlurBox->SetIsChecked(bMotionBlur);
	UpdateButtonSlate();
}

void UOptionWidget::OnHoveredMotionBlurButton()
{
	GraphicMenuNavIndex = 5;
	UpdateButtonSlate();
}

// Resolution Menu
void UOptionWidget::OnClickResolution1Button()
{
	int32 x = 1024, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution1Button()
{
	SubMenuNavIndex = 0;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution2Button()
{
	int32 x = 1152, y = 864;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution2Button()
{
	SubMenuNavIndex = 1;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution3Button()
{
	int32 x = 1280, y = 720;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution3Button()
{
	SubMenuNavIndex = 2;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution4Button()
{
	int32 x = 1280, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution4Button()
{
	SubMenuNavIndex = 3;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution5Button()
{
	int32 x = 1280, y = 800;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution5Button()
{
	SubMenuNavIndex = 4;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution6Button()
{
	int32 x = 1280, y = 960;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution6Button()
{
	SubMenuNavIndex = 5;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution7Button()
{
	int32 x = 1280, y = 1024;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution7Button()
{
	SubMenuNavIndex = 6;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution8Button()
{
	int32 x = 1366, y = 768;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution8Button()
{
	SubMenuNavIndex = 7;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution9Button()
{
	int32 x = 1600, y = 900;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution9Button()
{
	SubMenuNavIndex = 8;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickResolution10Button()
{
	int32 x = 1920, y = 1080;
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredResolution10Button()
{
	SubMenuNavIndex = 9;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickWindowedButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetFullscreenMode(EWindowMode::Windowed);
		UserSetting->ApplySettings(true);
		UserSetting->SaveSettings();
	}
	// SetCurrentGraphicsMode(GraphicsType::None);
}

void UOptionWidget::OnHoveredWindowedButton()
{
	SubMenuNavIndex = 0;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickWinFullButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		UserSetting->ApplySettings(true);
		UserSetting->SaveSettings();
	}
	
}

void UOptionWidget::OnHoveredWinFullButton()
{
	SubMenuNavIndex = 1;
	UpdateButtonSlate();
}

void UOptionWidget::OnClickFullScreenButton()
{
	UGameUserSettings* UserSetting = nullptr;
	if (GEngine)
		UserSetting = GEngine->GetGameUserSettings();
	if (UserSetting)
	{
		UserSetting->SetFullscreenMode(EWindowMode::Fullscreen);
		UserSetting->ApplySettings(true);
		UserSetting->SaveSettings();
	}
}

void UOptionWidget::OnHoveredFullScreenButton()
{
	SubMenuNavIndex = 2;
	UpdateButtonSlate();
}

void UOptionWidget::SetBrightness(int32 value)
{
	Brightness += value;

	BrightnessBar->SetPercent(0.5f);
}

void UOptionWidget::UpdateButtonSlate()
{
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("NavIndex: %d, GraphicsNavIndex: %d, SubNavIndex: %d"), MenuNavIndex, GraphicMenuNavIndex, SubMenuNavIndex));*/
	if (CurrentMode == OptionType::None)
	{
		switch (MenuNavIndex)
		{
		case 0:
			PlaySettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 2:
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 3:
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		default:
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		}
	}
	else
	{
		PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}

	if (CurrentMode == OptionType::GraphicSetting)
	{
		if (CurrentGraphicsMode == GraphicsType::None)
		{
			switch (GraphicMenuNavIndex)
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

		if (CurrentGraphicsMode == GraphicsType::Resolution)
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

		if (CurrentGraphicsMode == GraphicsType::Window)
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
}

void UOptionWidget::SetCurrentMode(OptionType ModeType)
{
	CurrentMode = ModeType;
}

void UOptionWidget::SetCurrentGraphicsMode(GraphicsType ModeType)
{
	CurrentGraphicsMode = ModeType;
}

FReply UOptionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	uint32 KeyNum = InKeyEvent.GetKeyCode();
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("KeyCode: %d"), KeyNum));*/
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		if (CurrentMode == OptionType::None)
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickPlaySettingButton();
				break;
			case 1:
				OnClickGraphicsSettingButton();
				break;
			case 2:
				OnClickKeyMappingButton();
				break;
			case 3:
				OnClickBackButton();
				break;
			}
		}
		else if (CurrentMode == OptionType::GraphicSetting)
		{
			if (CurrentGraphicsMode == GraphicsType::None)
			{
				switch (GraphicMenuNavIndex)
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
				// SetCurrentGraphicsMode(GraphicsType::Option);
				// MenuNumber = MenuBox->GetChildrenCount();
			}
			else if (CurrentGraphicsMode == GraphicsType::Resolution)
			{
				switch (SubMenuNavIndex)
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
				MenuNumber = MainBox->GetChildrenCount();
				SetCurrentGraphicsMode(GraphicsType::None);
			}
			else if (CurrentGraphicsMode == GraphicsType::Window)
			{
				switch (SubMenuNavIndex)
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
				MenuNumber = MainBox->GetChildrenCount();
				SetCurrentGraphicsMode(GraphicsType::None);
			}
			else if (CurrentGraphicsMode == GraphicsType::Brightness)
			{
				MenuNumber = MainBox->GetChildrenCount();
				SetCurrentGraphicsMode(GraphicsType::None);
			}
		}
	}
	else
	{
		if (CurrentMode == OptionType::None)
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
		}
		else if (CurrentMode == OptionType::GraphicSetting)
		{
			if (CurrentGraphicsMode == GraphicsType::None)
			{
				if (KeyType == "S" || KeyType == "Down")
				{
					GraphicMenuNavIndex++;
					if (GraphicMenuNavIndex >= MenuNumber)
					{
						GraphicMenuNavIndex = 0;
					}
				}
				else if (KeyType == "W" || KeyType == "Up")
				{
					GraphicMenuNavIndex--;
					if (GraphicMenuNavIndex < 0)
					{
						GraphicMenuNavIndex = MenuNumber - 1;
					}
				}
				else if (KeyType == "A" || KeyType == "Left")
				{
					/*if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Back")));*/
					SetCurrentGraphicsMode(GraphicsType::Option);
					SetCurrentMode(OptionType::None);
					PlayAnimation(BackOptionAnim);
					MenuNumber = MenuBox->GetChildrenCount();
				}
			}
			else if (CurrentGraphicsMode == GraphicsType::Resolution)
			{
				if (KeyType == "S" || KeyType == "Down")
				{
					SubMenuNavIndex += 5;
					if (SubMenuNavIndex >= MenuNumber)
					{
						SubMenuNavIndex -= 10;
					}
				}
				else if (KeyType == "W" || KeyType == "Up")
				{
					SubMenuNavIndex -= 5;
					if (SubMenuNavIndex < 0)
					{
						SubMenuNavIndex += 10;
					}
				}
				else if (KeyType == "A" || KeyType == "Left")
				{
					SubMenuNavIndex--;
					if (SubMenuNavIndex < 0)
					{
						SubMenuNavIndex = MenuNumber - 1;
					}
					/*if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Back")));*/
				}
				else if (KeyType == "D" || KeyType == "Right")
				{
					SubMenuNavIndex++;
					if (SubMenuNavIndex >= MenuNumber)
					{
						SubMenuNavIndex = 0;
					}
				}
			}
			else if (CurrentGraphicsMode == GraphicsType::Window)
			{
				if (KeyType == "D" || KeyType == "Right")
				{
					SubMenuNavIndex++;
					if (SubMenuNavIndex >= MenuNumber)
					{
						SubMenuNavIndex = 0;
					}
				}
				else if (KeyType == "A" || KeyType == "Left")
				{
					SubMenuNavIndex--;
					if (SubMenuNavIndex < 0)
					{
						SubMenuNavIndex = MenuNumber - 1;
					}
				}
			}
			else if (CurrentGraphicsMode == GraphicsType::Brightness)
			{
				if (KeyType == "S" || KeyType == "Down" || KeyType == "D" || KeyType == "Right")
				{
					SetBrightness(1);
				}
				else if (KeyType == "W" || KeyType == "Up" || KeyType == "A" || KeyType == "Left")
				{
					SetBrightness(-1);
				}
			}
		}
		UpdateButtonSlate();
	}

	/*if (CurrentMode == OptionType::None)
	{
		if (KeyType == "Enter")
		{
			switch (MenuNavIndex)
			{
			case 0:
				OnClickPlaySettingButton();
				break;
			case 1:
				OnClickGraphicsSettingButton();
				break;
			case 2:
				OnClickKeyMappingButton();
				break;
			case 3:
				OnClickBackButton();
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
	}*/

	//else if (CurrentMode == OptionType::GraphicSetting)
	//{
	//	if (CurrentGraphicsMode == GraphicsType::None)
	//	{
	//		if (KeyType == "Enter")
	//		{
	//			switch (GraphicMenuNavIndex)
	//			{
	//			case 0:
	//				OnClickResolutionButton();
	//				break;
	//			case 1:
	//				OnClickWindowModeButton();
	//				break;
	//			case 2:
	//				OnClickBrightButton();
	//				break;
	//			case 3:
	//				OnClickAntiAliasingButton();
	//				break;
	//			case 4:
	//				OnClickShadowButton();
	//				break;
	//			case 5:
	//				OnClickMotionBlurButton();
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (KeyType == "S" || KeyType == "Down")
	//			{
	//				GraphicMenuNavIndex++;
	//				if (GraphicMenuNavIndex >= MenuNumber)
	//				{
	//					GraphicMenuNavIndex = 0;
	//				}
	//			}
	//			else if (KeyType == "W" || KeyType == "Up")
	//			{
	//				GraphicMenuNavIndex--;
	//				if (GraphicMenuNavIndex < 0)
	//				{
	//					GraphicMenuNavIndex = MenuNumber - 1;
	//				}
	//			}
	//			else if (KeyType == "A" || KeyType == "Left")
	//			{
	//				if (GEngine)
	//					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Back")));
	//				SetCurrentGraphicsMode(GraphicsType::Option);
	//				SetCurrentMode(OptionType::None);
	//				MenuNumber = MenuBox->GetChildrenCount();
	//				
	//				/*GraphicMenuNavIndex++;
	//				if (GraphicMenuNavIndex >= MenuNumber)
	//				{
	//					GraphicMenuNavIndex = 0;
	//				}*/
	//			}
	//			/*else if (KeyType == "W" || KeyType == "Up")
	//			{
	//				GraphicMenuNavIndex--;
	//				if (GraphicMenuNavIndex < 0)
	//				{
	//					GraphicMenuNavIndex = MenuNumber - 1;
	//				}
	//			}*/
	//			UpdateButtonSlate();
	//		}
	//	}
	//	else if (CurrentGraphicsMode == GraphicsType::Resolution)
	//	{
	//		if (KeyType == "Enter")
	//		{
	//			switch (GraphicMenuNavIndex)
	//			{
	//			case 0:
	//				OnClickResolution1Button();
	//				break;
	//			case 1:
	//				OnClickResolution2Button();
	//				break;
	//			case 2:
	//				OnClickResolution3Button();
	//				break;
	//			case 3:
	//				OnClickResolution4Button();
	//				break;
	//			case 4:
	//				OnClickResolution5Button();
	//				break;
	//			case 5:
	//				OnClickResolution6Button();
	//				break;
	//			case 6:
	//				OnClickResolution7Button();
	//				break;
	//			case 7:
	//				OnClickResolution8Button();
	//				break;
	//			case 8:
	//				OnClickResolution9Button();
	//				break;
	//			case 9:
	//				OnClickResolution10Button();
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (KeyType == "S" || KeyType == "Down")
	//			{
	//				SubMenuNavIndex += 5;
	//				if (SubMenuNavIndex >= MenuNumber)
	//				{
	//					SubMenuNavIndex -= 10;
	//				}
	//			}
	//			else if (KeyType == "W" || KeyType == "Up")
	//			{
	//				SubMenuNavIndex -= 5;
	//				if (SubMenuNavIndex < 0)
	//				{
	//					SubMenuNavIndex += 10;
	//				}
	//			}
	//			else if (KeyType == "A" || KeyType == "Left")
	//			{
	//				SubMenuNavIndex--;
	//				if (SubMenuNavIndex < 0)
	//				{
	//					SubMenuNavIndex = MenuNumber - 1;
	//				}
	//				if (GEngine)
	//					GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Back")));
	//				/*SetCurrentGraphicsMode(GraphicsType::None);
	//				MenuNumber = MainBox->GetChildrenCount();*/
	//				/*GraphicMenuNavIndex++;
	//				if (GraphicMenuNavIndex >= MenuNumber)
	//				{
	//					GraphicMenuNavIndex = 0;
	//				}*/
	//			}
	//			else if (KeyType == "D" || KeyType == "Right")
	//			{
	//				SubMenuNavIndex++;
	//				if (SubMenuNavIndex >= MenuNumber)
	//				{
	//					SubMenuNavIndex = 0;
	//				}
	//			}
	//			UpdateButtonSlate();
	//		}
	//	}
	//	else if (CurrentGraphicsMode == GraphicsType::Window)
	//	{
	//		if (KeyType == "Enter")
	//		{
	//			switch (MenuNavIndex)
	//			{
	//			case 0:
	//				OnClickWindowedButton();
	//				break;
	//			case 1:
	//				OnClickWinFullButton();
	//				break;
	//			case 2:
	//				OnClickFullScreenButton();
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (KeyType == "D" || KeyType == "Right")
	//			{
	//				SubMenuNavIndex++;
	//				if (SubMenuNavIndex >= MenuNumber)
	//				{
	//					SubMenuNavIndex = 0;
	//				}
	//			}
	//			else if (KeyType == "A" || KeyType == "Left")
	//			{
	//				SubMenuNavIndex--;
	//				if (SubMenuNavIndex < 0)
	//				{
	//					SubMenuNavIndex = MenuNumber - 1;
	//				}
	//			}
	//			//else if (KeyType == "A" || KeyType == "Left")
	//			//{
	//			//	if (GEngine)
	//			//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("Back")));
	//			//	SetCurrentGraphicsMode(GraphicsType::None);
	//			//	MenuNumber = MainBox->GetChildrenCount();
	//			//	/*GraphicMenuNavIndex++;
	//			//	if (GraphicMenuNavIndex >= MenuNumber)
	//			//	{
	//			//		GraphicMenuNavIndex = 0;
	//			//	}*/
	//			//}
	//			UpdateButtonSlate();
	//		}
	//	}
	//	else if (CurrentGraphicsMode == GraphicsType::Brightness)
	//	{
	//		if (KeyType == "Enter")
	//		{
	//			SetCurrentGraphicsMode(GraphicsType::None);
	//		}
	//		else
	//		{
	//			if (KeyType == "S" || KeyType == "Down" || KeyType == "D" || KeyType == "Right")
	//			{
	//				SetBrightness(1);
	//			}
	//			else if (KeyType == "W" || KeyType == "Up" || KeyType == "A" || KeyType == "Left")
	//			{
	//				SetBrightness(-1);
	//			}
	//			UpdateButtonSlate();
	//		}
	//	}
	//}

	return reply;
}