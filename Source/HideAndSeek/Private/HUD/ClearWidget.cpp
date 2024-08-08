// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ClearWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelManager/HorrorGameGameInstance.h"
#include "Sound/SoundCue.h"

// 클리어 위젯 생성자
void UClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	//NextStageButton = Cast<UButton>(GetWidgetFromName(TEXT("NextStage")));
	//BackToMainButton = Cast<UButton>(GetWidgetFromName(TEXT("BackToMain")));

	// 현재 레벨을 가져오고, 기록 갱신 텍스트는 안 보이게 생성함.
	CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	RecordBreakingText->SetVisibility(ESlateVisibility::Collapsed);

	// 클리어 타임이 없을 경우 클리어타임을 가져옴.
	if (ClearTime != nullptr)
	{
		if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			//FText ClearedTime = GameInstance->GetClearTime(ClearedChapter);
			ClearTime->SetText(ClearedTime);
		}
	}

	// 각 버튼 별로 클릭과 마우스 호버 시 작동할 함수를 바인딩함.
	if (BackToMainButton)
	{
		BackToMainButton->OnClicked.AddDynamic(this, &UClearWidget::OnClickBackToMain);
		BackToMainButton->OnHovered.AddDynamic(this, &UClearWidget::OnHoveredBackToMain);
	}

	if (CurrentLevel != "Level1")
	{
		if (NextStageButton)
		{
			NextStageButton->OnClicked.AddDynamic(this, &UClearWidget::OnClickNextStage);
			NextStageButton->OnHovered.AddDynamic(this, &UClearWidget::OnHoveredNextStage);
			MenuNumber = 2;
			bHaveNextLevel = true;
		}
	}
	// 현재 챕터가 Level1(마지막 챕터)일 경우, 안 보이게 설정하고 버튼 개수도 1개로 줄임.
	else
	{
		NextStageButton->SetVisibility(ESlateVisibility::Hidden);
		MenuNumber = 1;
		bHaveNextLevel = false;
	}

	UpdateButtonSlate();
}

// 다음 스테이지로 이동 버튼을 눌렀을 때 작동할 함수.
void UClearWidget::OnClickNextStage()
{
	// 현재 챕터가 프롤로그일 경우
	if (CurrentLevel == "Prologue")
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 다음 레벨을 엶.
		FString NextLevelName = TEXT("/Game/Levels/GameLevel/Level1");
		UGameplayStatics::OpenLevel(GetWorld(), *NextLevelName);
	}
}

// 메인 화면으로 이동 버튼을 눌렀을 때 작동할 함수.
void UClearWidget::OnClickBackToMain()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 메인 화면이 출력될 레벨을 엶.
	FString levelName = TEXT("/Game/Levels/GameLevel/Start");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

// 다음 레벨로 이동 버튼 위에 마우스를 올리면 작동할 함수.
void UClearWidget::OnHoveredNextStage()
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

// 메인 화면으로 이동 버튼 위에 마우스를 올리면 작동할 함수.
void UClearWidget::OnHoveredBackToMain()
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

// 클리어 타임를 설정하는 함수
void UClearWidget::SetClearTime(FString inClearTime)
{
	ClearTime->SetText(FText::FromString(inClearTime));
}

// 클리어 타임 기록을 갱신했을 때, 경신했다는 텍스트를 보여줄 함수.
void UClearWidget::SetRecordBreakingText(bool inIsVisible)
{
	if (inIsVisible)
	{
		RecordBreakingText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

// 버튼의 상태를 업데이트하는 함수.
void UClearWidget::UpdateButtonSlate()
{
	// 다음 레벨로 이동 가능한 경우
	if (bHaveNextLevel)
	{
		switch (MenuNavigationIndex)
		{
			// 현재 버튼 인덱스가 0번이면 다음 챕터로 이동 버튼만 흰색으로 설정함.
			case 0:
			{
				NextStageButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				BackToMainButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				return;
			}
			// 현재 버튼 인덱스가 1번이면 메인 화면으로 이동 버튼만 흰색으로 설정함.
			case 1:
			{
				NextStageButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				BackToMainButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				return;
			}
		}
	}
	// 다음 레벨이 없는 경우, 메인 화면으로 이동 버튼만 흰색으로 설정함.
	else
	{
		BackToMainButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UClearWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		// 다음 챕터가 있다면
		if (bHaveNextLevel)
		{
			switch (MenuNavigationIndex)
			{
				// 현재 버튼 인덱스가 0이면, 다음 챕터로 이동 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnClickNextStage();
					break;
				}
				// 현재 버튼 인덱스가 1이면, 메인 화면으로 이동 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnClickBackToMain();
					break;
				}
			}
		}
		// 다음 챕터가 없다면 메인 화면으로 이동 버튼을 클릭한 효과를 줌.
		else
		{
			OnClickBackToMain();
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

			// 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어가면 0으로 바꿈.
			MenuNavigationIndex++;
			if (MenuNavigationIndex >= MenuNumber)
			{
				MenuNavigationIndex = 0;
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

			// 버튼 인덱스를 1 감소시키고, 0 미만인 경우, 최대 인덱스로 바꿈.
			MenuNavigationIndex--;
			if (MenuNavigationIndex < 0)
			{
				MenuNavigationIndex = MenuNumber - 1;
			}
		}

		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}