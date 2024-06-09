// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/StageSelectWidget.h"
#include "Components/Button.h"
#include "HorrorGameGameInstance.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "StartGameMode.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"
#include "Animation/WidgetAnimation.h"
 
// 챕터 선택 위젯 생성자
void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus(); // 키보드 입력을 받을 수 있게 Keyboard Focus를 설정해둠.

	bIsButtonClicked = false; // false로 초기화함.

	// Game Instance에서 HorrorGameGameInstance로 캐스팅한 후 클리어한 챕터와 클리어 데이터를 가져옴.
	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		ClearedChapter = GameInstance->GetClearedChapter(); // 클리어한 챕터를 가져오고
		ClearData = GameInstance->GetAllClearData(); // 구조체 형태의 클리어 데이터 값을 가져옴
	}

	// 블루프린트 클래스로부터 ChapterPanel이라는 위젯 오브젝트를 가져와 MenuBox에 할당함.
	//MenuBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("ChapterPanel")));

	// 플레이어가 선택 가능한 메뉴의 개수는 현재는 3개.
	MenuNumber = 3;
	
	/*
	// 삭제해도 됨. 선택가능한 메뉴를 나타내기 위한 배열 변수임.
	CanButtonSelect.Init(false, MenuNumber); 

	for (int i = 0; i < ButtonNum; ++i)
	{
		CanButtonSelect[i] = true;
	}
	CanButtonSelect[MenuNumber - 1] = true;
	*/

	/*ChapOneButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapOneBtn")));
	ChapTwoButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapTwoBtn")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	LevelImg = Cast<UImage>(GetWidgetFromName(TEXT("StageSampleImg")));*/

	// 각 버튼 별로 클릭, 마우스 호버 시 작동할 함수를 바인딩 해줌.
	if (nullptr != ChapOneButton) // ChapOneButton이 null이 아니면, 콜백 함수 바인드 시킴
	{
		ChapOneButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapOneButton);
		ChapOneButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapOneButton);
	}

	if (nullptr != ChapTwoButton) // ChapTwoButton이 null이 아니면, 콜백 함수 바인드 시킴.
	{
		ChapTwoButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapTwoButton);
		ChapTwoButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapTwoButton);
		
		// 챕터가 열려 있는 경우, 챕터명을 설정하고, 아닐 경우 ???로 설정함.
		if (ClearData[1].bIsOpened)
		{
			ChapTwoText->SetText(NSLOCTEXT("UStageSelectWidget", "ChapTwoText", "School of Labyrinth"));
		}
		else
		{
			ChapTwoText->SetText(NSLOCTEXT("UStageSelectWidget", "ChapUnLock", "???"));
		}
	}

	if (nullptr != BackButton) // BackButton이 null이 아니면, 콜백 함수 바인드 시킴.
	{
		BackButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredBackButton);
	}
	
	// 버튼 상태를 업데이트해놓아서, 현재 ChapOneButton에 커서가 있는 상황으로 만듦
	UpdateButtonSlate();
}

// Chapter One 버튼을 클릭하였을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickChapOneButton()
{
	// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
	if (!bIsButtonClicked)
	{
		// 스테이지 선택 사운드가 있다면, 사운드를 재생함.
		if (IsValid(StageSelectSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
		}

		// 기존의 FadeOutWidget 이벤트를 초기화해주고 OnMoveChapOne이라는 함수를 바인드 시켜줌.
		FadeoutWidgetAnimationEvent.Clear();
		FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapOne"))));

		// 바인딩한 콜백 함수를 Fadeout 애니메이션이 종료된 이후 재생되도록 바인딩하고 Fadeout 애니메이션을 재생함.
		BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
		PlayAnimation(Fadeout);

		// 추가로 버튼을 클릭하고 애니메이션이 재생되는 중간에는 다른 버튼 클릭 및 호버를 할 수 없게 만들어야 함.
		bIsButtonClicked = true;
	}
}

// Chapter Two 버튼을 클릭했을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickChapTwoButton()
{
	//챕터가 열려있는 경우에만 수행되도록 설정
	if (ClearData[1].bIsOpened) 
	{
		// 다른 버튼을 클릭한 상태가 아닌 경우 실행됨.
		if (!bIsButtonClicked)
		{
			// 스테이지 선택 사운드가 있다면, 사운드를 재생함.
			if (IsValid(StageSelectSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
			}

			// 기존의 FadeOutWidget 이벤트를 초기화해주고, OnMoveChapTwo라는 함수를 바인딩해줌.
			FadeoutWidgetAnimationEvent.Clear();
			FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapTwo"))));

			// 바인딩한 콜백 함수를 Fadeout 애니메이션이 종료된 이후에 재생되도록 애니메이션과 바인딩하고 Fadeout을 재생함.
			BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
			PlayAnimation(Fadeout);

			// 버튼을 클릭하고 애니메이션이 재생되는 동안에는 다른 버튼 클릭 및 호버를 불가능하게 설정해야 함.
			bIsButtonClicked = true;
		}
	}
	// 챕터가 안 열린 경우에 에러 음이 나도록 설정해야할 듯함.

}

/*
// Chapter Three 버튼을 클릭하였을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickChapTrheeButton()
{
	if(ClearData[2].bIsOpened) // 칩터가 열려있는 경우에만 수행되도록 설정
	{

		// 스테이지 선택 사운드가 있다면, 사운드를 재생함.
		if (IsValid(StageSelectSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
		}

		// 기존의 FadeOutWidget 이벤트를 초기화해주고 OnMoveChapThree라는 함수를 바인드 시켜줌.
		FadeoutWidgetAnimationEvent.Clear();
		FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapThree"))));

		// 바인딩한 콜백 함수를 Fadeout 애니메이션이 종료된 이후 재생되도록 바인딩하고 Fadeout 애니메이션을 재생함.
		BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
		PlayAnimation(Fadeout);

		// 추가로 버튼을 클릭하고 애니메이션이 재생되는 중간에는 다른 버튼 클릭 및 호버를 할 수 없게 만들어야 함.
	}
	// 챕터가 안 열린 경우에 에러 음이 나도록 설정.
	else
	{
		// 에러 음이 존재한다면, 사운드를 재생함.
		if(IsValid(ButtonErrorSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonErrorSound);
		}
	}
}

// Chapter Four 버튼을 클릭했을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickChapFourButton()
{
	if (ClearData[3].bIsOpened) //챕터가 열려있는 경우에만 수행되도록 설정
	{
		// 스테이지 선택 사운드가 있다면, 사운드를 재생함.
		if (IsValid(StageSelectSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
		}

		// 기존의 FadeOutWidget 이벤트를 초기화해주고, OnMoveChapFour라는 함수를 바인딩해줌.
		FadeoutWidgetAnimationEvent.Clear();
		FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapFour"))));

		// 바인딩한 콜백 함수를 Fadeout 애니메이션이 종료된 이후에 재생되도록 애니메이션과 바인딩하고 Fadeout을 재생함.
		BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
		PlayAnimation(Fadeout);

		// 버튼을 클릭하고 애니메이션이 재생되는 동안에는 다른 버튼 클릭 및 호버를 불가능하게 설정해야 함.
	}
	// 챕터가 안 열린 경우에 에러 음이 나도록 설정.
	else
	{
		// 에러 음이 존재한다면, 사운드를 재생함.
		if(IsValid(ButtonErrorSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonErrorSound);
		}
	}
}

// Chapter Five 버튼을 클릭했을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickChapFiveButton()
{
	if (ClearData[4].bIsOpened) //챕터가 열려있는 경우에만 수행되도록 설정
	{
		// 스테이지 선택 사운드가 있다면, 사운드를 재생함.
		if (IsValid(StageSelectSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
		}

		// 기존의 FadeOutWidget 이벤트를 초기화해주고, OnMoveChapFive라는 함수를 바인딩해줌.
		FadeoutWidgetAnimationEvent.Clear();
		FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapFive"))));

		// 바인딩한 콜백 함수를 Fadeout 애니메이션이 종료된 이후에 재생되도록 애니메이션과 바인딩하고 Fadeout을 재생함.
		BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
		PlayAnimation(Fadeout);

		// 버튼을 클릭하고 애니메이션이 재생되는 동안에는 다른 버튼 클릭 및 호버를 불가능하게 설정해야 함.
	}
	// 챕터가 안 열린 경우에 에러 음이 나도록 설정.
	else
	{
		// 에러 음이 존재한다면, 사운드를 재생함.
		if(IsValid(ButtonErrorSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonErrorSound);
		}
	}
}
*/

// Back 버튼을 클릭했을 때 호출할 콜백 함수.
void UStageSelectWidget::OnClickBackButton()
{
	// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
	if (!bIsButtonClicked)
	{
		// 버튼 클릭 사운드가 있다면, 사운드를 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// GameMode 클래스를 가져오되 StartGameMode 클래스로 캐스팅해 가져옴.
		// StageSelectWidget은 StartLevel에서 호출되는데 StartLevel의 메인 GameMode는 StartGameMode 클래스로 설정됨. 즉 캐스팅 에러가 발생할 확률 없음.
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		// StartWidget으로 변경하도록 ChangeWidget 호출함.
		GameMode->ChangeWidget(WidgetType::StartWidget);

		// 버튼을 클릭하고 애니메이션이 재생되는 동안에는 호버를 불가능하게 설정해야 함.
		bIsButtonClicked = true;
	}
}

// Fadeout 애니메이션 종료 후 호출할 콜백 함수.
void UStageSelectWidget::OnMoveChapOne()
{
	// Prologue라는 레벨의 상대 주소를 가져와서 해당 레벨을 OpenLevel로 호출함.
	FString levelName = TEXT("/Game/Levels/GameLevel/Prologue");
	//if(GameInstance)
	//	GameInstance->StopSound();
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

// Fadeout 애니메이션 종료 후 호출할 콜백 함수.
void UStageSelectWidget::OnMoveChapTwo()
{
	// Level1이라는 레벨의 상대 주소를 가져와서 해당 레벨을 OpenLevel로 호출함.
	FString levelName = TEXT("/Game/Levels/GameLevel/Level1");
	/*if(GameInstance)
		GameInstance->StopSound();*/
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

/*
// Fadeout 애니메이션 종료 후 호출할 콜백 함수.
void UStageSelectWidget::OnMoveChapThree()
{
	// Level2라는 레벨의 상대 주소를 가져와서 해당 레벨을 OpenLevel로 호출함.
	FString levelName = TEXT("/Game/Levels/GameLevel/Level2");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

// Fadeout 애니메이션 종료 후 호출할 콜백 함수.
void UStageSelectWidget::OnMoveChapFour()
{
	// Level3이라는 레벨의 상대 주소를 가져와서 해당 레벨을 OpenLevel로 호출함.
	FString levelName = TEXT("/Game/Levels/GameLevel/Level3");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}
// Fadeout 애니메이션 종료 후 호출할 콜백 함수.
void UStageSelectWidget::OnMoveChapFive()
{
	// Level4라는 레벨의 상대 주소를 가져와서 해당 레벨을 OpenLevel로 호출함.
	FString levelName = TEXT("/Game/Levels/GameLevel/Level4");
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

*/

// Deprecated
//void UStageSelectWidget::OnMoveBack()
//{
//	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//
//	GameMode->ChangeWidget(WidgetType::StartWidget);
//}

// Chapter One 버튼에 마우스가 호버되었을 때 호출할 콜백 함수
void UStageSelectWidget::OnHoveredChapOneButton()
{
	// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
	if (!bIsButtonClicked)
	{
		// 버튼 이동 음이 있다면 재생함
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 메뉴 인덱스를 0으로 설정하고 버튼 상태를 업데이트함.
		MenuNavigationIndex = 0;
		UpdateButtonSlate();
	}
}

// Chapter Two 버튼에 마우스가 호버되었을 때 호출할 콜백 함수,
void UStageSelectWidget::OnHoveredChapTwoButton()
{
	// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
	if (!bIsButtonClicked)
	{
		// 버튼 이동 음이 있다면 재생함
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 메뉴 인덱스를 1로 설정하고 버튼 상태를 업데이트함.
		MenuNavigationIndex = 1;
		UpdateButtonSlate();
	}
}

// Back 버튼에 마우스가 호버되었을 때 호출할 콜백 함수.
void UStageSelectWidget::OnHoveredBackButton()
{
	// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
	if (!bIsButtonClicked)
	{
		// 버튼 이동 음이 있다면 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 메뉴 인덱스를 2로 설정하고 버튼 상태를 업데이트함.
		MenuNavigationIndex = 2;// (챕터 추가가 완료되면 증가시켜야 함)
		UpdateButtonSlate();
	}
}

// 버튼 상태를 업데이트할 함수.
void UStageSelectWidget::UpdateButtonSlate()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SelectWidget: MenuNavigation: %d"), MenuNavigationIndex));
	
	// 메뉴 인덱스마다 버튼 상태를 업데이트하도록 설정함.
	switch (MenuNavigationIndex)
	{
		case 0: // 인덱스가 0일 때
		{
			// Chap One 버튼의 색을 하얀색으로 설정하고, 나머지 버튼의 색을 회색으로 설정함.
			ChapOneButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 중앙의 챕터 미리보기 이미지를 0번 인덱스의 이미지(챕터 1의 이미지)로 설정함.
			StageSampleImage->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
			return;
		}
		case 1: // 인덱스가 1일 때
		{
			// Chap Two 버튼의 색을 하얀색으로 설정하고, 나머지 버튼의 색을 회색으로 설정함.
			ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ChapTwoButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			if (ClearData[1].bIsOpened) //챕터가 열려있는 경우
			{
				// 중앙의 챕터 미리보기 이미지를 1번 인덱스의 이미지(챕터 2의 이미지)로 설정함.
				StageSampleImage->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
			}
			else // 챕터가 열려있지 않은 경우
			{
				// 중앙의 챕터 미리보기 이미지를 마지막 인덱스(투명한 이미지)로 설정함.
				StageSampleImage->SetBrushFromTexture(LevelSample[2]);
			}
			return;
		}
		case 2: // 인덱스가 2일 때
		{
			// Back 버튼의 색을 하얀색으로 설정하고, 나머지 버튼의 색을 회색으로 설정함.
			ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

			// 중앙의 챕터 미리보기 이미지를 마지막 인덱스(투명한 이미지)로 설정함.
			StageSampleImage->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
			return;
		}
	}
}

// 키보드 입력도 받을 것이기 때문에 사용
// Key가 Pressed될 경우 호출할 콜백 함수
FReply UStageSelectWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 키 코드를 가져와 String 형태로 바꿔서 저장함. 어떤 키를 입력받은 것인지 알기 위해 변환함.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();
	
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("KeyType: %s"), *KeyType));*/

	if (KeyType == "Enter") // 만약 엔터를 입력받은 경우
	{
		switch (MenuNavigationIndex) // 현재 메뉴 인덱스를 봄
		{
			case 0: // 만약 메뉴 인덱스가 0일 경우
			{
				// Chap One 버튼을 클릭했을 때 호출할 콜백 함수를 호출함.
				OnClickChapOneButton();
				return reply;
			}
			case 1: // 만약 메뉴 인덱스가 1일 경우
			{
				// Chap Two 버튼을 클릭했을 때 호출할 콜백 함수를 호출함.
				OnClickChapTwoButton();
				return reply;
			}
			case 2: // 만약 메뉴 인덱스가 2일 경우
			{
				// Back 버튼을 클릭했을 때 호출할 콜백 함수를 호출함.
				OnClickBackButton();
				return reply;
			}
		}
	}
	// 엔터가 아닌 다른 키를 입력받았을 때
	else
	{
		// 입력받은 키가 S나 Down 화살표일 경우
		if (KeyType == "S" || KeyType == "Down")
		{
			// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
			if (!bIsButtonClicked)
			{
				// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				MenuNavigationIndex++; // 일단 인덱스를 증가시키고
				if (MenuNavigationIndex > ClearedChapter) // 챕터 개수 이상으로 인덱스 초과시
				{
					MenuNavigationIndex = MenuNumber - 1; // Back으로
				}
				if (MenuNavigationIndex >= MenuNumber)
				{
					MenuNavigationIndex = 0; // 총 버튼 수를 초과하면 다시 0으로
				}
			}
		}
		// 입력받은 키가 W나 Up 화살표일 경우
		else if (KeyType == "W" || KeyType == "Up")
		{
			// 다른 버튼을 클릭한 상태가 아닌 경우에 실행됨.
			if (!bIsButtonClicked)
			{
				// 버튼 이동 음이 null 포인터가 아니면 해당 음 재생.
				if (IsValid(ButtonMoveSound))
				{
					UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
				}

				MenuNavigationIndex--; // 일단 인덱스 감소시키고
				if (MenuNavigationIndex > ClearedChapter) // 감소시켰는데 챕터 개수 이상으로 인덱스 초과 시
				{
					MenuNavigationIndex = ClearedChapter;
				}
				if (MenuNavigationIndex < 0)
				{
					MenuNavigationIndex = MenuNumber - 1; // 0 미만으로 떨어지면 다시 최대 인덱스로
				}
			}
		}

		// 버튼 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}