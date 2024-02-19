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

void UGraphicsOption::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(NewOptionAnim);

	this->SetKeyboardFocus();

	EndDelegate.BindDynamic(this, &UGraphicsOption::AnimationFinished);
	BindToAnimationFinished(BackOptionAnim, EndDelegate);

	/*ResolutionButton = Cast<UButton>(GetWidgetFromName(TEXT("Resolution")));
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
	//Resolution9Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution9Btn")));
	//Resolution10Button = Cast<UButton>(GetWidgetFromName(TEXT("Resolution10Btn")));
	WindowedButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowBtn")));
	// WinFullButton = Cast<UButton>(GetWidgetFromName(TEXT("WindowFullBtn")));
	FullScreenButton = Cast<UButton>(GetWidgetFromName(TEXT("FullScreenBtn")));
	AntiAliasBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("AntiCheckbox")));
	ShadowBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("ShadowCheckbox")));
	MotionBlurBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("MotionBlurCheckbox")));
	BrightnessBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("BrightBar")));

	MainBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("GraphicsSettingPanel")));
	ResolutionPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("ResolutionType")));
	WindowModeBox = Cast<UHorizontalBox>(GetWidgetFromName(TEXT("WindowModePanel")));*/

	if (IsValid(Resolution))
	{
		Resolution->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolutionButton);
		Resolution->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolutionButton);
		Resolution->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolutionButton);
	}
	if (IsValid(WindowMode))
	{
		WindowMode->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWindowModeButton);
		WindowMode->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWindowModeButton);
		WindowMode->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWindowModeButton);
	}
	if (IsValid(BrightButton))
	{
		BrightButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickBrightButton);
		BrightButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredBrightButton);
		BrightButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredBrightButton);
	}
	if (IsValid(AntiAliasingBtn))
	{
		AntiAliasingBtn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickAntiAliasingButton);
		AntiAliasingBtn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredAntiAliasingButton);
		AntiAliasingBtn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredAntiAliasingButton);
	}
	if (IsValid(ShadowBtn))
	{
		ShadowBtn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickShadowButton);
		ShadowBtn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredShadowButton);
		ShadowBtn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredShadowButton);
	}
	if (IsValid(MotionBlurBtn))
	{
		MotionBlurBtn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickMotionBlurButton);
		MotionBlurBtn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredMotionBlurButton);
		MotionBlurBtn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredMotionBlurButton);
	}
	if (IsValid(Resolution1Btn))
	{
		Resolution1Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution1Button);
		Resolution1Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution1Button);
		Resolution1Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution1Button);
	}
	if (IsValid(Resolution2Btn))
	{
		Resolution2Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution2Button);
		Resolution2Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution2Button);
		Resolution2Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution2Button);
	}
	if (IsValid(Resolution3Btn))
	{
		Resolution3Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution3Button);
		Resolution3Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution3Button);
		Resolution3Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution3Button);
	}
	if (IsValid(Resolution4Btn))
	{
		Resolution4Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution4Button);
		Resolution4Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution4Button);
		Resolution4Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution4Button);
	}
	if (IsValid(Resolution5Btn))
	{
		Resolution5Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution5Button);
		Resolution5Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution5Button);
		Resolution5Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution5Button);
	}
	if (IsValid(Resolution6Btn))
	{
		Resolution6Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution6Button);
		Resolution6Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution6Button);
		Resolution6Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution6Button);
	}
	if (IsValid(Resolution7Btn))
	{
		Resolution7Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution7Button);
		Resolution7Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution7Button);
		Resolution7Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution7Button);
	}
	if (IsValid(Resolution8Btn))
	{
		Resolution8Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution8Button);
		Resolution8Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution8Button);
		Resolution8Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution8Button);
	}
	/*if (IsValid(Resolution9Btn))
	{
		Resolution9Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution9Button);
		Resolution9Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution9Button);
		Resolution9Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution9Button);
	}
	if (IsValid(Resolution10Btn))
	{
		Resolution10Btn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickResolution10Button);
		Resolution10Btn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredResolution10Button);
		Resolution10Btn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredResolution10Button);
	}*/
	if (IsValid(WindowBtn))
	{
		WindowBtn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWindowedButton);
		WindowBtn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWindowedButton);
		WindowBtn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWindowedButton);
	}
	/*if (IsValid(WinFullButton))
	{
		WinFullButton->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickWinFullButton);
		WinFullButton->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredWinFullButton);
		WinFullButton->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredWinFullButton);
	}*/
	if (IsValid(FullScreenBtn))
	{
		FullScreenBtn->OnClicked.AddDynamic(this, &UGraphicsOption::OnClickFullScreenButton);
		FullScreenBtn->OnHovered.AddDynamic(this, &UGraphicsOption::OnHoveredFullScreenButton);
		FullScreenBtn->OnUnhovered.AddDynamic(this, &UGraphicsOption::OnUnhoveredFullScreenButton);
	}
	if (IsValid(GraphicsSettingPanel))
	{
		MenuNumber = GraphicsSettingPanel->GetChildrenCount();
	}

	if (GEngine)
	{
		UserSetting = GEngine->GetGameUserSettings(); // 유저 세팅 파일 가져오기
	}

	if (UserSetting->GetAntiAliasingQuality() == 3)
	{
		bIsAntiAliasing = true;
	}
	else
	{
		bIsAntiAliasing = false;
	}
	AntiCheckbox->SetIsChecked(bIsAntiAliasing);

	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Current Shadow Quality: %d"), UserSetting->GetShadowQuality()));
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

	FIntPoint IntPoint = UserSetting->GetScreenResolution();
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Current Resolution: %dx%d"), IntPoint.X, IntPoint.Y));
	CurrentResolution = FString::FromInt(IntPoint.X) + TEXT("x") + FString::FromInt(IntPoint.Y);
	CheckCurrentResolution(CurrentResolution);

	// GameInstance로부터 밝기 감마 값과 모션 블러 설정값 가져오기(GameInstance는 실행 시 Save 데이터를 읽어옴)
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		Brightness = GameInstance->GetBrightGamma();
		bMotionBlur = GameInstance->GetMotionBlur();
	} 

	MotionBlurCheckbox->SetIsChecked(bMotionBlur);
	BrightBar->SetPercent(Brightness / 5.0f);
	SetCurrentMode(GraphicsType::None);
	UpdateButtonSlate();
}

// Main Menu
void UGraphicsOption::OnClickResolutionButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SubMenuNavIndex = 0;
	SetCurrentMode(GraphicsType::Resolution);
	if (IsValid(ResolutionType))
	{
		MenuNumber = ResolutionType->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredResolutionButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 0;
		UpdateButtonSlate();
		//Resolution->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGraphicsOption::OnUnhoveredResolutionButton()
{
	Resolution->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGraphicsOption::OnClickWindowModeButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SubMenuNavIndex = 0;
	SetCurrentMode(GraphicsType::Window);
	if (IsValid(WindowModePanel))
	{
		MenuNumber = WindowModePanel->GetChildrenCount();
	}
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredWindowModeButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 1;
		UpdateButtonSlate();
		//WindowMode->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGraphicsOption::OnUnhoveredWindowModeButton()
{
	WindowMode->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGraphicsOption::OnClickBrightButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	SetCurrentMode(GraphicsType::Brightness);
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredBrightButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 2;
		UpdateButtonSlate();	
		//BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGraphicsOption::OnUnhoveredBrightButton()
{
	BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGraphicsOption::OnClickAntiAliasingButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	bIsAntiAliasing = !bIsAntiAliasing;
	AntiCheckbox->SetIsChecked(bIsAntiAliasing);

	if (UserSetting)
	{
		int32 Value = bIsAntiAliasing ? 3 : 0; // ShadowSetting이 true면 3, false면 0으로 설정
		UserSetting->SetAntiAliasingQuality(Value);
		UserSetting->ApplyNonResolutionSettings();
		UserSetting->SaveSettings();
	}

	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredAntiAliasingButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 3;
		UpdateButtonSlate();
		//AntiAliasingBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGraphicsOption::OnUnhoveredAntiAliasingButton()
{
	AntiAliasingBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGraphicsOption::OnClickShadowButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	bShadowSetting = !bShadowSetting;
	ShadowCheckbox->SetIsChecked(bShadowSetting);

	if (UserSetting)
	{
		int32 Value = bShadowSetting ? 3 : 0; // ShadowSetting이 true면 3, false면 1으로 설정
		UserSetting->SetShadowQuality(Value);
		UserSetting->ApplyNonResolutionSettings();
		UserSetting->SaveSettings();
	}

	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredShadowButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 4;
		UpdateButtonSlate();
		//ShadowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

void UGraphicsOption::OnUnhoveredShadowButton()
{
	ShadowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

void UGraphicsOption::OnClickMotionBlurButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	bMotionBlur = !bMotionBlur;
	MotionBlurCheckbox->SetIsChecked(bMotionBlur);
	FString Command = bMotionBlur ? TEXT("r.DefaultFeature.MotionBlur 1") : TEXT("r.DefaultFeature.MotionBlur 0"); // true면 1, false면 0으로 설정

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		bool bResult = GameInstance->MotionBlurSaveLogic(bMotionBlur);
		if (bResult)
		{
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				PlayerController->ConsoleCommand(Command);
			}
			//GetWorld()->Exec(GetWorld(), *Command); // 저장을 완료해야 모션 블러 세팅 완료
		}
		else
		{
	//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Error to save MotionBlur settings")));
		}
	}
	
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredMotionBlurButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::None)
	{
		MenuNavIndex = 5;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredMotionBlurButton()
{
	MotionBlurBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
}

// Resolution Menu
void UGraphicsOption::OnClickResolution1Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 854, y = 480;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
	bResolution1 = true;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution1Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution1Button()
{
	if (bResolution1)
	{
		Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution2Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 1366, y = 768;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
	bResolution1 = false;
	bResolution2 = true;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution2Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 1;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution2Button()
{
	if (bResolution2)
	{
		Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution3Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 1600, y = 900;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}
	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = true;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution3Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 2;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution3Button()
{
	if (bResolution3)
	{
		Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution4Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 1920, y = 1080;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}

	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = true;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution4Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 3;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution4Button()
{
	if (bResolution4)
	{
		Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution5Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 1280, y = 800;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}

	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = true;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution5Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 4;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution5Button()
{
	if (bResolution5)
	{
		Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution6Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 1920, y = 1200;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}

	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = true;
	bResolution7 = false;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution6Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 5;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution6Button()
{
	if (bResolution6)
	{
		Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution7Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 2560, y = 1080;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}

	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = true;
	bResolution8 = false;
}

void UGraphicsOption::OnHoveredResolution7Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 6;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution7Button()
{
	if (bResolution7)
	{
		Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::OnClickResolution8Button()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	int32 x = 3440, y = 1440;

	if (UserSetting)
	{
		UserSetting->SetScreenResolution(FIntPoint(x, y));
		UserSetting->ApplyResolutionSettings(true);
		UserSetting->SaveSettings();
	}

	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = true;
}

void UGraphicsOption::OnHoveredResolution8Button()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Resolution)
	{
		SubMenuNavIndex = 7;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredResolution8Button()
{
	if (bResolution8)
	{
		Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

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
//		Resolution9Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution9Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
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
//		Resolution10Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		Resolution10Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

void UGraphicsOption::OnClickWindowedButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (UserSetting)
	{
		UserSetting->SetFullscreenMode(EWindowMode::Windowed);
		UserSetting->ApplySettings(false);
	}
	bWindowed = true;
	bFullScreen = false;
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredWindowedButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Window)
	{
		SubMenuNavIndex = 0;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredWindowedButton()
{
	if (bWindowed)
	{
		WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

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
//		WinFullBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
//	}
//	else
//	{
//		WinFullBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
//	}
//}

void UGraphicsOption::OnClickFullScreenButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (UserSetting)
	{
		UserSetting->SetFullscreenMode(EWindowMode::WindowedFullscreen);
		UserSetting->ApplySettings(true);
		UserSetting->SaveSettings();
	}

	bWindowed = false;
	bFullScreen = true;
	UpdateButtonSlate();
}

void UGraphicsOption::OnHoveredFullScreenButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	if (CurrentMode == GraphicsType::Window)
	{
		SubMenuNavIndex = 2;
		UpdateButtonSlate();
	}
}

void UGraphicsOption::OnUnhoveredFullScreenButton()
{
	if (bFullScreen)
	{
		FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
	}
	else
	{
		FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
	}
}

void UGraphicsOption::SetBrightness(float Value)
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	Brightness += Value;
	if (Brightness <= 0.5f)
	{
		Brightness = 0.5f;
	}
	if (Brightness >= 5.0f)
	{
		Brightness = 5.0f;
	}

	//BrightBar->SetPercent(Brightness / 5.f);
	BrightBar->SetPercent((Brightness - 0.5f) / 4.5f);

	FString CVarGamma = TEXT("gamma ") + FString::Printf(TEXT("%f"), Brightness);

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		bool Result = GameInstance->BrightGammaSaveLogic(Brightness);
		if (Result)
		{
			UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass());
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PlayerController)
			{
				PlayerController->ConsoleCommand(CVarGamma);
			}
			else
			{
		//		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("No Player Controller!")));
			}
			//GetWorld()->Exec(GetWorld(), *CVarGamma);
		}
		else
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Error to save Brightness settings")));
		}
	}

}

void UGraphicsOption::UpdateButtonSlate()
{
	if (CurrentMode == GraphicsType::None)
	{
		switch (MenuNavIndex)
		{
		case 0:
			Resolution->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 1:
			Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 2:
			Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 3:
			Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 4:
			Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		case 5:
			Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			MotionBlurBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
	}
	else
	{
		Resolution->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		WindowMode->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BrightButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		AntiAliasingBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ShadowBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		MotionBlurBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
	}
	
	if (CurrentMode == GraphicsType::Resolution)
	{
		SelectResolutionMode(bResolution1, bResolution2, bResolution3, bResolution4, bResolution5,
			bResolution6, bResolution7, bResolution8, SubMenuNavIndex);
	}
	else
	{
		SelectResolutionMode(bResolution1, bResolution2, bResolution3, bResolution4, bResolution5,
			bResolution6, bResolution7, bResolution8, -1);
	}
	
	if (CurrentMode == GraphicsType::Window)
	{
		SelectWindowMode(bWindowed, bFullScreen, SubMenuNavIndex);
	}
	else
	{
		SelectWindowMode(bWindowed, bFullScreen, -1);
	}
}

void UGraphicsOption::SelectWindowMode(bool bWindowedMode, bool bFullScreenMode, int32 CurrentIndex)
{
	if (bWindowedMode)
	{
		if (CurrentIndex == 0)
		{
			WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 0)
		{
			WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			WindowBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bFullScreenMode)
	{
		if (CurrentIndex == 1)
		{
			FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 1)
		{
			FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			FullScreenBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}
}

void UGraphicsOption::SelectResolutionMode(bool bResolution1Mode, bool bResolution2Mode, bool bResolution3Mode, bool bResolution4Mode,
	bool bResolution5Mode, bool bResolution6Mode, bool bResolution7Mode, bool bResolution8Mode, int32 CurrentIndex)
{
	if (bResolution1Mode)
	{
		if (CurrentIndex == 0)
		{
			Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 0)
		{
			Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution1Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution2Mode)
	{
		if (CurrentIndex == 1)
		{
			Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 1)
		{
			Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution2Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution3Mode)
	{
		if (CurrentIndex == 2)
		{
			Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 2)
		{
			Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution3Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution4Mode)
	{
		if (CurrentIndex == 3)
		{
			Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 3)
		{
			Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution4Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution5Mode)
	{
		if (CurrentIndex == 4)
		{
			Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 4)
		{
			Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution5Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution6Mode)
	{
		if (CurrentIndex == 5)
		{
			Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 5)
		{
			Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution6Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution7Mode)
	{
		if (CurrentIndex == 6)
		{
			Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 6)
		{
			Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution7Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}

	if (bResolution8Mode)
	{
		if (CurrentIndex == 7)
		{
			Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
		else
		{
			Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 0.8f));
		}
	}
	else
	{
		if (CurrentIndex == 7)
		{
			Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		}
		else
		{
			Resolution8Btn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.6f));
		}
	}
}

void UGraphicsOption::SetCurrentMode(GraphicsType ModeType)
{
	CurrentMode = ModeType;
	if (ModeType == GraphicsType::None)
	{
		MenuNumber = GraphicsSettingPanel->GetChildrenCount();
	}
	else if (ModeType == GraphicsType::Resolution)
	{
		MenuNumber = ResolutionType->GetChildrenCount();
	}
	else if (ModeType == GraphicsType::Window)
	{
		MenuNumber = WindowModePanel->GetChildrenCount();
	}
}

void UGraphicsOption::CheckCurrentResolution(const FString ForCheckResolution)
{
	bResolution1 = false;
	bResolution2 = false;
	bResolution3 = false;
	bResolution4 = false;
	bResolution5 = false;
	bResolution6 = false;
	bResolution7 = false;
	bResolution8 = false;

	if (ForCheckResolution.Equals(TEXT("854x480")))
	{
		bResolution1 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("1366x768")))
	{
		bResolution2 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("1600x900")))
	{
		bResolution3 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("1920x1080")))
	{
		bResolution4 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("1280x800")))
	{
		bResolution5 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("1920x1200")))
	{
		bResolution6 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("2560x1080")))
	{
		bResolution7 = true;
	}
	else if (ForCheckResolution.Equals(TEXT("3440x1440")))
	{
		bResolution8 = true;
	}
}

void UGraphicsOption::AnimationFinished()
{
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

FReply UGraphicsOption::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	uint32 KeyNum = InKeyEvent.GetKeyCode();
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, FString::Printf(TEXT("KeyCode: %d"), KeyNum));*/
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();


	if (KeyType == "Enter")
	{
		if (CurrentMode == GraphicsType::None)
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
		else if (CurrentMode == GraphicsType::Resolution)
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
				/*case 8:
					OnClickResolution9Button();
					break;
				case 9:
					OnClickResolution10Button();
					break;*/
			}
			SetCurrentMode(GraphicsType::None);
		}
		else if (CurrentMode == GraphicsType::Window)
		{
			switch (SubMenuNavIndex)
			{
			case 0:
				OnClickWindowedButton();
				break;
				/*case 1:
					OnClickWinFullButton();
					break;*/
					//case 2:
			case 1:
				OnClickFullScreenButton();
				break;
			}
			SetCurrentMode(GraphicsType::None);
		}
		else if (CurrentMode == GraphicsType::Brightness)
		{
			SetCurrentMode(GraphicsType::None);
		}
	}
	else
	{
		if (CurrentMode == GraphicsType::None)
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
		else if (CurrentMode == GraphicsType::Resolution)
		{
			if (KeyType == "S" || KeyType == "Down")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				SubMenuNavIndex += 4;
				if (SubMenuNavIndex >= MenuNumber)
				{
					SubMenuNavIndex -= 8;
				}
			}
			else if (KeyType == "W" || KeyType == "Up")
			{
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}
				SubMenuNavIndex -= 4;
				if (SubMenuNavIndex < 0)
				{
					SubMenuNavIndex += 8;
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
			else if (KeyType == "D" || KeyType == "Right")
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
		}
		else if (CurrentMode == GraphicsType::Window)
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
		else if (CurrentMode == GraphicsType::Brightness)
		{
			if (KeyType == "S" || KeyType == "Down" || KeyType == "A" || KeyType == "Left")
			{
				SetBrightness(-0.1);
			}
			else if (KeyType == "W" || KeyType == "Up" || KeyType == "D" || KeyType == "Right")
			{
				SetBrightness(0.1);
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}