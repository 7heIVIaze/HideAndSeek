// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ArchiveWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "Sound/SoundCue.h"
#include "HUD/ArchiveDocsWidget.h"
#include "HUD/HintWidget.h"
#include "PauseWidget.h"

// 생성자
void UArchiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	// 각 버튼마다 클릭과 마우스 호버할 때 작동할 함수 바인딩시킴.
	Item->OnClicked.AddDynamic(this, &UArchiveWidget::OnItemButtonClick);
	Item->OnHovered.AddDynamic(this, &UArchiveWidget::OnItemButtonHovered);

	Document->OnClicked.AddDynamic(this, &UArchiveWidget::OnDocumentButtonClick);
	Document->OnHovered.AddDynamic(this, &UArchiveWidget::OnDocumentButtonHovered);

	Letter->OnClicked.AddDynamic(this, &UArchiveWidget::OnLetterButtonClick);
	Letter->OnHovered.AddDynamic(this, &UArchiveWidget::OnLetterButtonHovered);

	Hint->OnClicked.AddDynamic(this, &UArchiveWidget::OnHintButtonClick);
	Hint->OnHovered.AddDynamic(this, &UArchiveWidget::OnHintButtonHovered);

	Exit->OnClicked.AddDynamic(this, &UArchiveWidget::OnExitButtonClick);
	Exit->OnHovered.AddDynamic(this, &UArchiveWidget::OnExitButtonHovered);

	// 버튼의 전체 개수는 5개이고, 최초 버튼 인덱스는 0으로 설정
	NavIndex = 5;
	CurrentIndex = 0;

	if (bIsStartGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("From Start Game Mode"));
	}

	UpdateButtonSlate();
}

// 나가기 버튼을 눌렀을 때 작동할 함수
void UArchiveWidget::OnExitButtonClick()
{
	// 버튼 클릭 음 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 시작 화면에서 들어온 경우
	if (bIsStartGameMode)
	{
		// 게임 모드 클래스를 가져와서 Start Widget 클래스로 변경해줌.
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->ChangeWidget(WidgetType::StartWidget);

	}
	// 인게임 중에서 들어온 경우
	else
	{
		// Pause Widget을 호출해 뷰포트에 출력하고, 이 위젯은 뷰포트에서 제거함
		UPauseWidget* NewWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidget);
		NewWidget->AddToViewport();

		RemoveFromParent();
	}
}

// 아이템 문서 버튼을 눌렀을 경우 작동할 함수.
void UArchiveWidget::OnItemButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// ArchiveDocs 위젯(각 문서 내역 확인 위젯)을 불러와 뷰포트에 출력함.
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);

	// 이 때, 불러온 위젯은 아이템 위젯으로 설정함.
	NewWidget->ArchiveMode = EArchiveModes::AM_Item;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;

	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

// 문서(신문, 일기) 버튼을 눌렀을 경우 작동할 함수. 
void UArchiveWidget::OnDocumentButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// ArchiveDocs 위젯(각 문서 내역 확인 위젯)을 불러와 뷰포트에 출력함.
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);
	
	// 이 때, 불러온 위젯은 문서 위젯으로 설정함.
	NewWidget->ArchiveMode = EArchiveModes::AM_Document;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;

	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

// 편지 문서 버튼을 클릭한 경우 작동할 함수.
void UArchiveWidget::OnLetterButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// ArchiveDocs 위젯(각 문서 내역 확인 위젯)을 불러와 뷰포트에 출력함.
	UArchiveDocsWidget* NewWidget = CreateWidget<UArchiveDocsWidget>(GetWorld(), DetailWidget);

	// 이 때, 불러온 위젯은 편지 위젯으로 설정함.
	NewWidget->ArchiveMode = EArchiveModes::AM_Letter;
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;

	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

// 힌트 문서 버튼을 클릭한 경우 작동할 함수.
void UArchiveWidget::OnHintButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	
	// Hint 위젯(도움말 확인 위젯)을 불러와 뷰포트에 출력함.
	UHintWidget* NewWidget = CreateWidget<UHintWidget>(GetWorld(), HintWidget);
	NewWidget->AddToViewport();
	NewWidget->bIsStartGameMode = bIsStartGameMode;

	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(NewWidget);
	}
	RemoveFromParent();
}

// 나가기 버튼 위에 마우스를 올린 경우 작동할 함수.
void UArchiveWidget::OnExitButtonHovered()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 4로 변경하고, 버튼의 상태를 업데이트 함.
	CurrentIndex = 4;
	UpdateButtonSlate();
}

// 아이템 버튼 위에 마우스를 올린 경우 작동할 함수.
void UArchiveWidget::OnItemButtonHovered()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 0으로 설정하고, 버튼의 상태를 업데이트 함.
	CurrentIndex = 0;
	UpdateButtonSlate();
}

// 문서 버튼 위에 마우스를 올린 경우 작동할 함수.
void UArchiveWidget::OnDocumentButtonHovered()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 1로 설정하고, 버튼의 상태를 업데이트 함.
	CurrentIndex = 1;
	UpdateButtonSlate();
}

// 편지 버튼 위에 마우스를 올린 경우 작동할 함수.
void UArchiveWidget::OnLetterButtonHovered()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
	CurrentIndex = 2;
	UpdateButtonSlate();
}

// 힌트 버튼 위에 마우스를 올린 경우 작동할 함수.
void UArchiveWidget::OnHintButtonHovered()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 버튼 인덱스를 3으로 설정하고, 버튼의 상태를 업데이트 함.
	CurrentIndex = 3;
	UpdateButtonSlate();
}

// 버튼의 상태를 업데이트하는 함수.
void UArchiveWidget::UpdateButtonSlate()
{
	switch (CurrentIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 아이템 버튼만 흰색으로 설정함.
		case 0:
		{
			Item->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 1일 때, 문서 버튼만 흰색으로 설정함.
		case 1:
		{
			Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Document->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 2일 때, 편지 버튼만 흰색으로 설정함.
		case 2:
		{
			Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Letter->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 3일 때, 힌트 버튼만 흰색으로 설정함.
		case 3:
		{
			Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Hint->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			return;
		}
		// 현재 버튼 인덱스가 4일 때, 나가기 버튼만 흰색으로 설정함.
		case 4:
		{
			Item->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Document->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Letter->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Hint->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			return;
		}
	}
}

// 키보드 키 입력을 담당할 함수.
FReply UArchiveWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키의 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 해당 키가 엔터일 경우
	if (KeyType == "Enter")
	{
		switch (CurrentIndex)
		{
			// 현재 버튼 인덱스가 0일 때, 아이템 버튼을 클릭했을 때 작동할 함수를 호출함.
			case 0:
			{
				OnItemButtonClick();
				break;
			}
			// 현재 버튼 인덱스가 1일 때, 문서 버튼을 클릭했을 때 작동할 함수를 호출함.
			case 1:
			{
				OnDocumentButtonClick();
				break;
			}
			// 현재 버튼 인덱스가 2일 때, 편지 버튼을 클릭했을 때 작동할 함수를 호출함.
			case 2:
			{
				OnLetterButtonClick();
				break;
			}
			// 현재 버튼 인덱스가 3일 때, 힌트 버튼을 클릭했을 때 작동할 함수를 호출함.
			case 3:
			{
				OnHintButtonClick();
				break;
			}
			// 현재 버튼 인덱스가 4일 때, 나가기 버튼을 클릭했을 때 작동할 함수를 호출함.
			case 4:
			{
				OnExitButtonClick();
				break;
			}
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// S 키나 아래 화살표 키인 경우
		if (KeyType == "S" || KeyType == "Down")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 증가시키고, 전체 버튼 수보다 넘어갈 경우, 0으로 설정함.
			CurrentIndex++;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex = 0;
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

			// 현재 버튼 인덱스를 1 감소시키고, 0보다 작을 경우, 최대 인덱스로 설정함.
			CurrentIndex--;
			if (CurrentIndex < 0)
			{
				CurrentIndex = NavIndex - 1;
			}
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}