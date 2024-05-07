// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/StageSelectWidget.h"
#include "Components/Button.h"
#include "HorrorGameGameInstance.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "StartGameMode.h"
#include "Components/Image.h"
#include "Sound/SoundCue.h"
#include "Animation/WidgetAnimation.h"

void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	if (UHorrorGameGameInstance* GameInstance = Cast<UHorrorGameGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		ClearedChapter = GameInstance->GetClearedChapter();
		ClearData = GameInstance->GetAllClearData();
	}

	//int32 ButtonNum = ClearedChapter + 1;
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("ButtonNum: %d"), ButtonNum));
	MenuBox = Cast<UVerticalBox>(GetWidgetFromName(TEXT("ChapterPanel")));
	MenuNumber = 3;
	//if (nullptr != MenuBox)
//	{
		// MenuNumber = MenuBox->GetChildrenCount();
	//	MenuNumber = 3;
	//}

	CanButtonSelect.Init(false, MenuNumber);

	/*for (int i = 0; i < ButtonNum; ++i)
	{
		CanButtonSelect[i] = true;
	}
	CanButtonSelect[MenuNumber - 1] = true;*/

	ChapOneButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapOneBtn")));
	ChapTwoButton = Cast<UButton>(GetWidgetFromName(TEXT("ChapTwoBtn")));
	BackButton = Cast<UButton>(GetWidgetFromName(TEXT("BackBtn")));
	LevelImg = Cast<UImage>(GetWidgetFromName(TEXT("StageSampleImg")));

	if (nullptr != ChapOneButton)
	{
		ChapOneButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapOneButton);
		ChapOneButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapOneButton);
	}

	if (nullptr != ChapTwoButton)
	{
		ChapTwoButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickChapTwoButton);
		ChapTwoButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredChapTwoButton);
		//if (ClearedChapter >= 1) // 이전 챕터를 클리어했을 경우
		if (ClearData[1].bIsOpened) //챕터가 열려있는 경우
		{
			ChapTwoText->SetText(NSLOCTEXT("UStageSelectWidget", "ChapTwoText", "School of Labyrinth"));
		}
		else
		{
			ChapTwoText->SetText(NSLOCTEXT("UStageSelectWidget", "ChapUnLock", "???"));
		}
	}

	if (nullptr != BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UStageSelectWidget::OnClickBackButton);
		BackButton->OnHovered.AddDynamic(this, &UStageSelectWidget::OnHoveredBackButton);
	}
	

	UpdateButtonSlate();
}

void UStageSelectWidget::OnClickChapOneButton()
{
	if (IsValid(StageSelectSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
	}
	FadeoutWidgetAnimationEvent.Clear();
	FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapOne"))));
	BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
	PlayAnimation(Fadeout);
	//Fadeout->Animation
	//FString levelName = TEXT("/Game/Levels/GameLevel/Prologue");
	//if(GameInstance)
	//	GameInstance->StopSound();
	//UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UStageSelectWidget::OnClickChapTwoButton()
{

	if (ClearData[1].bIsOpened) //챕터가 열려있는 경우
	{
		if (IsValid(StageSelectSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), StageSelectSound);
		}
		FadeoutWidgetAnimationEvent.Clear();
		FadeoutWidgetAnimationEvent.BindUFunction(this, FName(FString(TEXT("OnMoveChapTwo"))));
		BindToAnimationFinished(Fadeout, FadeoutWidgetAnimationEvent);
		PlayAnimation(Fadeout);
	}
	//FString levelName = TEXT("/Game/Levels/GameLevel/Level1");
	/*if(GameInstance)
		GameInstance->StopSound();*/
	//UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UStageSelectWidget::OnClickBackButton()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	GameMode->ChangeWidget(WidgetType::StartWidget);
}

void UStageSelectWidget::OnMoveChapOne()
{
	FString levelName = TEXT("/Game/Levels/GameLevel/Prologue");
	//if(GameInstance)
	//	GameInstance->StopSound();
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}

void UStageSelectWidget::OnMoveChapTwo()
{
	FString levelName = TEXT("/Game/Levels/GameLevel/Level1");
	/*if(GameInstance)
		GameInstance->StopSound();*/
	UGameplayStatics::OpenLevel(GetWorld(), *levelName);
}
//
//void UStageSelectWidget::OnMoveBack()
//{
//	auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//
//	GameMode->ChangeWidget(WidgetType::StartWidget);
//}

void UStageSelectWidget::OnHoveredChapOneButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 0;
	UpdateButtonSlate();
}

void UStageSelectWidget::OnHoveredChapTwoButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 1;
	UpdateButtonSlate();
}

void UStageSelectWidget::OnHoveredBackButton()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	MenuNavigationIndex = 2;
	UpdateButtonSlate();
}

void UStageSelectWidget::UpdateButtonSlate()
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("SelectWidget: MenuNavigation: %d"), MenuNavigationIndex));
	switch (MenuNavigationIndex)
	{
	case 0:
		ChapOneButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		break;
	case 1:
		ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		if (ClearData[1].bIsOpened) //챕터가 열려있는 경우
		{
			LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		}
		else
		{
			LevelImg->SetBrushFromTexture(LevelSample[2]);
		}
		break;
	case 2:
		ChapOneButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		ChapTwoButton->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		BackButton->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		LevelImg->SetBrushFromTexture(LevelSample[MenuNavigationIndex]);
		break;
	}
}

FReply UStageSelectWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();
	
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("KeyType: %s"), *KeyType));*/
	if (KeyType == "Enter")
	{
		switch (MenuNavigationIndex)
		{
		case 0:
			OnClickChapOneButton();
			break;
		case 1:
			OnClickChapTwoButton();
			break;
		case 2:
			OnClickBackButton();
			break;
		}
	}

	else
	{
		if (KeyType == "S" || KeyType == "Down")
		{
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
			//while (!CanButtonSelect[MenuNavigationIndex]) // 현재 가리키는 버튼이 사용 가능하면 종료함
			//{
			//	MenuNavigationIndex++; // 가리킬 수 있는 버튼일 때까지 인덱스 증가하고
			//	//if (MenuNavigationIndex > ClearedChapter) // 챕터 개수 이상으로 인덱스 초과시
			//	//{
			//	//	MenuNavigationIndex = MenuNumber - 1; // Back으로
			//	//}
			//	if (MenuNavigationIndex >= MenuNumber)
			//	{
			//		MenuNavigationIndex = 0; // 총 버튼 수를 초과하면 다시 0으로
			//	}
			//}
		}
		else if (KeyType == "W" || KeyType == "Up")
		{
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
			//while (!CanButtonSelect[MenuNavigationIndex]) // 현재 가리키는 버튼이 사용가능하면 종료함
			//{
			//	MenuNavigationIndex--; // 가리킬 수 잇는 버튼일 때까지 인덱스 감소시키고
			//	//if (MenuNavigationIndex > ClearedChapter) // 감소시켰는데 챕터 개수 이상으로 인덱스 초과 시
			//	//{
			//	//	MenuNavigationIndex = ClearedChapter;
			//	//}
			//	if (MenuNavigationIndex < 0)
			//	{
			//		MenuNavigationIndex = MenuNumber - 1; // 0 미만으로 떨어지면 다시 최대 인덱스로
			//	}
			//}
		}
		UpdateButtonSlate();
	}

	return reply;
}