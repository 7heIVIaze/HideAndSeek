// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/OptionWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "LevelManager/StartGameMode.h"
#include "HUD/StartWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Math/IntPoint.h"
#include "Engine/Console.h"
#include "Player/HorrorGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// 옵션 위젯 생성자.
void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	//PlaySettingButton = Cast<UButton>(GetWidgetFromName(TEXT("PlayOptionBtn")));
	//GraphicsSettingButton = Cast<UButton>(GetWidgetFromName(TEXT("GraphicOptionBtn")));
	////KeyMappingButton = Cast<UButton>(GetWidgetFromName(TEXT("KeyMappingBtn")));
	//BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackToMainBtn")));
	//MenuBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("OptionSelectBox")));
	//MenuLine = Cast<UBorder>(GetWidgetFromName(TEXT("Line")));

	// 각 버튼 별로 클릭, 마우스 호버 시 작동할 함수를 바인딩해줌.
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

	/*if (nullptr != KeyMappingButton)
	{
		KeyMappingButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickKeyMappingButton);
		KeyMappingButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredKeyMappingButton);
	}*/

	if (nullptr != BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UOptionWidget::OnHoveredBackButton);
	}
	
	// 전체 버튼의 개수는 3개이고, 버튼의 상태를 업데이트 함.
	MenuNumber = 3;
	
	UpdateButtonSlate();
	// SetCurrentMode(OptionType::None);
}

// 게임 플레이 설정 버튼을 클릭했을 때 작동할 함수.
void UOptionWidget::OnClickPlaySettingButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	
	// 게임모드 클래스를 불러와 게임 플레이 세팅 위젯을 뷰포트에 출력하고 이 위젯을 제거함.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::GameplaySettingWidget);
}

// 게임 플레이 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UOptionWidget::OnHoveredPlaySettingButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavIndex = 0;
	UpdateButtonSlate();
}

// 그래픽 설정 버튼을 클릭했을 때 작동할 함수.
void UOptionWidget::OnClickGraphicsSettingButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	
	// 게임 모드 클래스를 가져와 그래픽 설정 위젯을 뷰포트에 출력하고, 이 위젯을 제거함.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::GraphicsSettingWidget);
}

// 그래픽 설정 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UOptionWidget::OnHoveredGraphicsSettingButton()
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

// 현재는 Deprecated.
//void UOptionWidget::OnClickKeyMappingButton()
//{
//	if (IsValid(ButtonClickSound))
//	{
//		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
//	}
//	if (GEngine)
//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("KeyMapping Click"));
//	//PlayAnimation(NewOptionAnim);
//	// SetCurrentMode(OptionType::KeySetting);
//}

// 현재는 Deprecated.
//void UOptionWidget::OnHoveredKeyMappingButton()
//{
//	if (IsValid(ButtonMoveSound))
//	{
//		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
//	}
//	MenuNavIndex = 2;
//	UpdateButtonSlate();
//}

// 뒤로 가기 버튼을 클릭했을 때 작동할 함수.
void UOptionWidget::OnClickBackButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 게임 모드를 클래스를 가져와 시작 위젯을 뷰포트에 출력하고 이 위젯을 제거함.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::StartWidget);
}

// 뒤로 가기 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UOptionWidget::OnHoveredBackButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	//MenuNavIndex = 3;

	// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavIndex = 2;
	UpdateButtonSlate();
}

// 버튼의 상태를 업데이트하는 함수.
void UOptionWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 게임 플레이 설정 버튼만 흰색으로 설정함.
		case 0:
		{
			PlaySettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			//KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		}
		// 현재 버튼 인덱스가 1일 때, 그래픽 설정 버튼만 흰색으로 설정함.
		case 1:
		{
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			//KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		}
		// 현재 버튼 인덱스가 2일 때, 뒤로 가기 버튼만 흰색으로 설정함.
		case 2:
		{
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			//KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			break;
		}
		/*
		case 3:
		{
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			KeyMappingButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			break;
		}
		*/
		// 현재 버튼 인덱스가 없을 때, 설정
		default:
		{
			PlaySettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			GraphicsSettingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			//KeyMappingButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		}
	}
}

// DEPRECATED
//void UOptionWidget::SetCurrentMode(OptionType ModeType)
//{
//	CurrentMode = ModeType;
//}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UOptionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	uint32 KeyNum = InKeyEvent.GetKeyCode();
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		switch (MenuNavIndex)
		{
			// 현재 버튼 인덱스가 0일 때, 게임 플레이 설정 버튼을 클릭한 효과를 줌.
			case 0:
			{
				OnClickPlaySettingButton();
				break;
			}
			// 현재 버튼 인덱스가 1일 때, 그래픽 설정 버튼을 클릭한 효과를 줌.
			case 1:
			{
				OnClickGraphicsSettingButton();
				break;
			}
			// 현재 버튼 인덱스가 2일 때, 뒤로 가기 버튼을 클릭한 효과를 줌.
			case 2:
			{
				OnClickBackButton();
				break;
			}
			/*
			case 3:
			{
				OnClickKeyMappingButton();
				UpdateButtonSlate();
				break;
			}
			*/
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// S 키이거나 아래 화살표 키인 경우
		if (KeyType == "S" || KeyType == "Down")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 설정함.
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

			// 현재 버튼 인덱스를 1 감소시키고, 0 미만일 경우, 최대 인덱스로 설정함.
			MenuNavIndex--;
			if (MenuNavIndex < 0)
			{
				MenuNavIndex = MenuNumber - 1;
			}
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}