// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "DiedWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "HorrorGamePlayerController.h"
#include "Sound/SoundCue.h"

// 생성자
void UDiedWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	// RestartButton = Cast<UButton>(GetWidgetFromName(TEXT("RestartBtn")));
	// ExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ExitBtn")));

	// 각 버튼 별로 클릭과 마우스 호버 시 호출할 콜백 함수를 바인딩해줌,
	if (nullptr != RestartButton)
	{
		RestartButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickRestartButton);
		RestartButton->OnHovered.AddDynamic(this, &UDiedWidget::OnHoveredRestartButton);
	}

	if (nullptr != ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UDiedWidget::OnClickExitButton);
		ExitButton->OnHovered.AddDynamic(this, &UDiedWidget::OnHoveredExitButton);
	}

	UpdateButtonSlate();
}

// 재시작 버튼을 클릭했을 때 호출할 함수.
void UDiedWidget::OnClickRestartButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 레벨을 재시작함.
	auto playerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());
	playerController->RestartLevel();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

// 메인 화면으로 버튼을 클릭했을 때 호출할 함수.
void UDiedWidget::OnClickExitButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 메인 화면 UI가 호출될 레벨로 이동함.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Game/Levels/Start"));
}

// 재시작 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UDiedWidget::OnHoveredRestartButton()
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

// 메인 화면으로 버튼을 클릭했을 때 작동할 함수.
void UDiedWidget::OnHoveredExitButton()
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

// 버튼을 업데이트하는 함수.
void UDiedWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 재시작 버튼만 흰색으로 표시.
		case 0:
		{
			RestartButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 1일 때, 메인 화면으로 버튼만 흰색으로 표시.
		case 1:
		{
			RestartButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ExitButton->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
			return;
		}
	}
}

// 문서 획득 텍스트를 설정하는 함수.
void UDiedWidget::SetArchiveGetText(FText inText)
{
	// 텍스트 블록이 존재하면, 텍스트를 설정함.
	if (ArchiveGetText == nullptr)
	{
		return;
	}
	ArchiveGetText->SetText(inText);
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UDiedWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	FKey keyCode = InKeyEvent.GetKey();
	FString keyType = keyCode.ToString();

	// 엔터 키인 경우
	if (keyType == "Enter")
	{
		switch (MenuNavIndex)
		{
			// 현재 버튼 인덱스가 0일 때, 재도전 버튼을 클릭한 효과를 줌.
		case 0:
			{
				OnClickRestartButton();
				break;
			}
			// 현재 버튼 인덱스가 1일 때, 메인 화면으로 버튼을 클릭한 효과를 줌.
			case 1:
			{
				OnClickExitButton();
				break;
			}
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// S 키이거나 아래 화살표 키인 경우
		if (keyType == "S" || keyType == "Down")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 설정함.
			MenuNavIndex++;
			if (MenuNavIndex >= MenuNum)
			{
				MenuNavIndex = 0;
			}
		}
		// 입력받은 키가 W나 Up 화살표일 경우
		else if (keyType == "W" || keyType == "Up")
		{
			// 버튼 이동 음 재생.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			
			// 현재 버튼 인덱스를 1 감소시키고, 0 미만일 경우, 최대 인덱스로 설정함.
			MenuNavIndex--;
			if (MenuNavIndex < 0)
			{
				MenuNavIndex = MenuNum - 1;
			}
		}

		// 버튼 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}