// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/PauseWidget.h"
#include "Components/Button.h"
#include "Player/HorrorGamePlayerController.h"
#include "Player/HorrorGameCharacter.h"
#include "HUD/GameUI.h"
#include "Sound/SoundCue.h"
#include "HUD/ArchiveWidget.h"
#include "HUD/TimerWidget.h"

// 생성자
void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("ResumeBtn")));
	RetryButton = Cast<UButton>(GetWidgetFromName(TEXT("RetryBtn")));
	QuitButton = Cast<UButton>(GetWidgetFromName(TEXT("QuitBtn")));*/

	// 각 버튼 별로 클릭 및 마우스 호버 이벤트 발생 시 호출할 콜백 함수를 바인딩함.
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickResumeButton);
		ResumeButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredResumeButton);
	}

	if (nullptr != RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickRetryButton);
		RetryButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredRetryButton);
	}

	if (nullptr != QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickQuitButton);
		QuitButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredQuitButton);
	}

	ArchiveButton->OnClicked.AddDynamic(this, &UPauseWidget::OnClickArchiveButton);
	ArchiveButton->OnHovered.AddDynamic(this, &UPauseWidget::OnHoveredArchiveButton);
	UpdateButtonSlate();
}

// 재개 버튼을 클릭할 때 작동할 함수.
void UPauseWidget::OnClickResumeButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 뷰포트에서 이 위젯을 제거하고, 일시정지를 멈춤.
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());

	RemoveFromParent();

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);
}

// 문서 보관함 버튼을 클릭할 때 작동할 함수.
void UPauseWidget::OnClickArchiveButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 문서 보관함 위젯을 생성해 뷰포트에 출력함.
	UArchiveWidget* NewWidget = CreateWidget<UArchiveWidget>(GetWorld(), ArchiveWidget);
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = false;

	// 이 위젯을 뷰포트에서 제거함.
	RemoveFromParent();
}

// 재시작 버튼을 클릭할 때 작동할 함수.
void UPauseWidget::OnClickRetryButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 타이머 위젯의 타이머를 멈추고 레벨을 재시작하도록 함.
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());
	auto Player = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());

	if (Player)
	{
		Player->GameUIWidget->TimerWidget->StopTimer();
	}
	PlayerController->RestartLevel();
}

// 메인 화면으로 버튼을 클릭할 때 작동할 함수.
void UPauseWidget::OnClickQuitButton()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 타이머 위젯의 타이머를 멈추고 메인 화면으로 이동하도록 함.
	auto PlayerController = Cast<AHorrorGamePlayerController>(GetOwningPlayer());
	auto Player = Cast<AHorrorGameCharacter>(PlayerController->GetPawn());

	if (Player)
	{
		Player->GameUIWidget->TimerWidget->StopTimer();
	}
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

// 재개 버튼 위에 마우스를 올릴 때 작동할 함수.
void UPauseWidget::OnHoveredResumeButton()
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

// 문서 보관함 버튼 위에 마우스를 올릴 때 작동할 함수.
void UPauseWidget::OnHoveredArchiveButton()
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

// 재도전 버튼 위에 마우스를 올릴 때 작동할 함수.
void UPauseWidget::OnHoveredRetryButton()
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

// 메인 화면으로 버튼 위에 마우스를 올릴 때 작동할 함수.
void UPauseWidget::OnHoveredQuitButton()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 3으로 설정하고, 버튼의 상태를 업데이트 함.
	MenuNavIndex = 3;
	UpdateButtonSlate();
}

// 버튼의 상태를 업데이트하는 함수.
void UPauseWidget::UpdateButtonSlate()
{
	switch (MenuNavIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 재개 버튼만 흰색으로 설정함.
		case 0:
		{
			ResumeButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 1일 때, 문서 보관함 버튼만 흰색으로 설정함.
		case 1:
		{
			ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 2일 때, 재도전 버튼만 흰색으로 설정함.
		case 2:
		{
			ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			RetryButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 3일 때, 메인 화면으로 버튼만 흰색으로 설정함.
		case 3:
		{
			ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			QuitButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			return;
		}
		// 예외 상황.
		default:
		{
			ResumeButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ArchiveButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			RetryButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			QuitButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		}
	}
}

// 키보드 입력도 받을 것이기 때문에 사용
// Key가 Pressed될 경우 호출할 콜백 함수
FReply UPauseWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 키 코드를 가져와 String 형태로 바꿔서 저장함. 어떤 키를 입력받은 것인지 알기 위해 변환함.
	FKey keyCode = InKeyEvent.GetKey();
	FString keyType = keyCode.ToString();

	// 만약 엔터를 입력받은 경우
	if (keyType == "Enter")
	{
		switch (MenuNavIndex)
		{
			// 현재 버튼 인덱스가 0일 때, 재개 버튼을 클릭한 효과를 줌.
			case 0:
			{
				OnClickResumeButton();
				break;
			}
			// 현재 버튼 인덱스가 1일 때, 문서 보관함 버튼을 클릭한 효과를 줌.
			case 1:
			{
				OnClickArchiveButton();
				break;
			}
			// 현재 버튼 인덱스가 2일 때, 재도전 버튼을 클릭한 효과를 줌.
			case 2:
			{
				OnClickRetryButton();
				break;
			}
			// 현재 버튼 인덱스가 3일 때, 메인 화면으로 버튼을 클릭한 효과를 줌.
			case 3:
			{
				OnClickQuitButton();
				break;
			}
		}
	}
	// 엔터가 아닌 다른 키를 입력받았을 때
	else
	{
		// 입력받은 키가 S나 Down 화살표일 경우
		if (keyType == "S" || keyType == "Down")
		{
			// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 일단 인덱스를 증가시키고 최대 인덱스를 넘어서면 0으로 설정함.
			MenuNavIndex++;
			if (MenuNavIndex >= MenuNum)
			{
				MenuNavIndex = 0;
			}
		}
		// 입력받은 키가 W나 Up 화살표일 경우
		else if (keyType == "W" || keyType == "Up")
		{
			// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 일단 인덱스를 감소시키고 0 미만으로 떨어지면 최소 인덱스로 설정함.
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