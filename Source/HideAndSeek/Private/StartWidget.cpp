// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "StartWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "HorrorGameGameInstance.h"
#include "StartGameMode.h"
#include "HUD/StageSelectWidget.h"
#include "Sound/SoundCue.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();
	/*StartButton = Cast<UButton>(GetWidgetFromName(TEXT("StartBtn")));
	OptionButton = Cast<UButton>(GetWidgetFromName(TEXT("OptionBtn")));
	ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));
	Menu = Cast<UVerticalBox>(GetWidgetFromName(TEXT("MenuPanel")));*/

	// 각 버튼 별로 클릭 및 마우스 호버 이벤트 시 작동할 콜백 함수를 바인딩해줌.
	if (nullptr != StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickStartButton);
		StartButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredStartButton);
	}

	if (nullptr != OptionButton)
	{
		OptionButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickOptionButton);
		OptionButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredOptionButton);
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UStartWidget::OnClickExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredExitButton);
	}

	if (nullptr != ArchiveBtn)
	{
		ArchiveBtn->OnClicked.AddDynamic(this, &UStartWidget::OnClickArchiveButton);
		ArchiveBtn->OnHovered.AddDynamic(this, &UStartWidget::OnHoveredArchiveButton);
	}

	// 버튼 개수를 메뉴 패널로부터 가져옴.
	if (nullptr != Menu)
	{
		MenuNumber = Menu->GetChildrenCount();
	}

	UpdateButtonSlate();
}

// 시작 버튼을 클릭했을 때 작동할 함수.
void UStartWidget::OnClickStartButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 챕터 선택 위젯으로 현재 위젯을 바꿈.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::LevelSelectWidget);

}

// 옵션 설정 버튼을 클릭했을 때 작동할 함수.
void UStartWidget::OnClickOptionButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 옵션 선택 위젯으로 현재 위젯을 바꿈.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::OptionSettingWidget);
}

// 게임 종료 버튼을 클릭했을 때 작동할 함수.
void UStartWidget::OnClickExitButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 게임을 종료함.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

// 문서 보관함 버튼을 클릭했을 때 작동할 함수.
void UStartWidget::OnClickArchiveButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 문서 보관함 위젯으로 현재 위젯을 바꿈.
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ChangeWidget(WidgetType::ArchiveWidget);
}

// 시작 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UStartWidget::OnHoveredStartButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
}

// 옵션 선택 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UStartWidget::OnHoveredOptionButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavigationIndex = 2;
	UpdateButtonSlate();
}

// 게임 종료 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UStartWidget::OnHoveredExitButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 3으로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavigationIndex = 3;
	UpdateButtonSlate();
}

// 문서 보관함 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UStartWidget::OnHoveredArchiveButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 1로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

// 버튼의 상태를 업데이트할 함수.
void UStartWidget::UpdateButtonSlate()
{
	switch(MenuNavigationIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 시작 버튼만 흰색으로 설정함.
		case 0:
		{
			StartButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 1일 때, 문서 보관함 버튼만 흰색으로 설정함.
		case 1:
		{
			StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveBtn->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 2일 때, 옵션 설정 버튼만 흰색으로 설정함.
		case 2:
		{
			StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			OptionButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 3일 때, 게임 종료 버튼만 흰색으로 설정함.
		case 3:
		{
			StartButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveBtn->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			OptionButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			return;
		}
	}
}

// 키보드 입력도 받을 것이기 때문에 사용
// Key가 Pressed될 경우 호출할 콜백 함수
FReply UStartWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 키 코드를 가져와 String 형태로 바꿔서 저장함. 어떤 키를 입력받은 것인지 알기 위해 변환함.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 만약 엔터를 입력받은 경우
	if (KeyType == "Enter")
	{
		switch (MenuNavigationIndex)
		{
			// 현재 버튼 인덱스가 0일 때, 시작 버튼을 클릭한 효과를 줌.
			case 0:
			{
				OnClickStartButton();
				break;
			}
			// 현재 버튼 인덱스가 1일 때, 문서 보관함 버튼을 클릭한 효과를 줌.
			case 1:
			{
				OnClickArchiveButton();
				break;
			}
			// 현재 버튼 인덱스가 2일 때, 옵션 설정 버튼을 클릭한 효과를 줌.
			case 2:
			{
				OnClickOptionButton();
				break;
			}
			// 현재 버튼 인덱스가 3일 때, 게임 종료 버튼을 클릭한 효과를 줌.
			case 3:
			{	
				OnClickExitButton();
				break;
			}
		}
	}
	// 엔터가 아닌 다른 키를 입력받았을 때
	else
	{
		// 입력받은 키가 S나 Down 화살표일 경우
		if (KeyType == "S" || KeyType == "Down")
		{
			// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 일단 인덱스를 증가시키고 최대 인덱스를 넘어서면 0으로 설정함.
			MenuNavigationIndex++;
			if (MenuNavigationIndex >= MenuNumber)
			{
				MenuNavigationIndex = 0;
			}
		}
		// 입력받은 키가 W나 Up 화살표일 경우
		else if (KeyType == "W" || KeyType == "Up")
		{
			// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 일단 인덱스를 감소시키고 0 미만으로 떨어지면 최소 인덱스로 설정함.
			MenuNavigationIndex--;
			if (MenuNavigationIndex < 0)
			{
				MenuNavigationIndex = MenuNumber - 1;
			}
		}

		// 버튼 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}