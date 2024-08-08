// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/HintWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "LevelManager/StartGameMode.h"
#include "HUD/ArchiveWidget.h"
#include "Sound/SoundCue.h"

// 도움말 문서 보관함 위젯 생성자
void UHintWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

	// 모든 변수들 초기화
	CatchedByReaper = false;
	CatchedByRunner = false;
	CatchedByBrute = false;

	Archive1Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));
	Archive2Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));
	Archive3Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));
	Archive4Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));
	Archive5Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));
	Archive6Text->SetText(NSLOCTEXT("UHintWidget", "ArchiveNone", "???"));

	CurrentIndex = 0;
	HintState = EHintStates::AS_Search;
	CreaturePanel->SetVisibility(ESlateVisibility::Collapsed);
	HintPanel->SetVisibility(ESlateVisibility::Collapsed);
	NavIndex = 7;

	// 세이브 데이터로부터 데이터를 가져오는 로직
	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		// 리퍼에게 한 번 죽었을 경우, 리퍼 견문록이 해금됨.
		if (SaveData->CollectArchives.CatchedByReaper)
		{
			CatchedByReaper = true;
			Archive1Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Reaper", "Grim Reaper"));
		}

		// 러너에게 한 번 죽었을 경우, 러너 견문록이 해금됨.
		if (SaveData->CollectArchives.CatchedByRunner)
		{
			CatchedByRunner = true;
			Archive2Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Runner", "Runner"));
		}

		// 브루트에게 한 번 죽었을 경우, 브루트 견문록이 해금됨.
		if (SaveData->CollectArchives.CatchedByBrute)
		{
			CatchedByBrute = true;
			Archive3Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Brute", "Brute"));
		}

		// 섀도우에게 한 번 죽었을 경우, 섀도우 견문록이 해금됨.
		if (SaveData->CollectArchives.CatchedByShadow)
		{
			CatchedByShadow = true;
			Archive4Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Shadow", "Shadow"));
		}

		// 요괴 특징을 한 번 보았을 경우, 요괴 특징 문서가 해금됨.
		if (SaveData->CollectArchives.SeeCharacteristic)
		{
			SeeCharacteristic = true;
			Archive5Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Characteristic", "Creature Characteristic"));
		}

		// 회피 방법을 한 번 보았을 경우, 회피 방법 문서가 해금됨.
		if (SaveData->CollectArchives.SeeHowToEscape)
		{
			SeeHowToEscape = true;
			Archive6Text->SetText(NSLOCTEXT("UHintWidget", "Archive_HowToEscape", "How to escape"));
		}
	}

	// 각 버튼 별로 클릭과 마우스 호버 시 작동할 함수를 바인딩해줌.
	Archive1->OnClicked.AddDynamic(this, &UHintWidget::OnArchive1ButtonClick);
	Archive1->OnHovered.AddDynamic(this, &UHintWidget::OnArchive1ButtonHover);

	Archive2->OnClicked.AddDynamic(this, &UHintWidget::OnArchive2ButtonClick);
	Archive2->OnHovered.AddDynamic(this, &UHintWidget::OnArchive2ButtonHover);

	Archive3->OnClicked.AddDynamic(this, &UHintWidget::OnArchive3ButtonClick);
	Archive3->OnHovered.AddDynamic(this, &UHintWidget::OnArchive3ButtonHover);

	Archive4->OnClicked.AddDynamic(this, &UHintWidget::OnArchive4ButtonClick);
	Archive4->OnHovered.AddDynamic(this, &UHintWidget::OnArchive4ButtonHover);

	Archive5->OnClicked.AddDynamic(this, &UHintWidget::OnArchive5ButtonClick);
	Archive5->OnHovered.AddDynamic(this, &UHintWidget::OnArchive5ButtonHover);

	Archive6->OnClicked.AddDynamic(this, &UHintWidget::OnArchive6ButtonClick);
	Archive6->OnHovered.AddDynamic(this, &UHintWidget::OnArchive6ButtonHover);

	Exit->OnClicked.AddDynamic(this, &UHintWidget::OnExitButtonClick);
	Exit->OnHovered.AddDynamic(this, &UHintWidget::OnExitButtonHover);

	CreatureExit->OnClicked.AddDynamic(this, &UHintWidget::OnCreatureExitButtonClick);
	HintExit->OnClicked.AddDynamic(this, &UHintWidget::OnHintExitButtonClick);

	UpdateButtonSlate();
}

// 뒤로 가기 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnExitButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 이전 위젯(문서 보관함 위젯)을 뷰포트에 출력하고, 이 위젯을 뷰포트에서 제거함/
		UArchiveWidget* FromWidget = CreateWidget<UArchiveWidget>(GetWorld(), ParentWidget);
		FromWidget->AddToViewport();
		auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FromWidget->SetUserFocus(Controller);
		FromWidget->SetKeyboardFocus();
		FromWidget->bIsStartGameMode = bIsStartGameMode;
		if (bIsStartGameMode)
		{
			auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			GameMode->SetCurrentWidget(FromWidget);
		}
		RemoveFromParent();
	}
}

// 뒤로 가기 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnExitButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 현재 버튼 인덱스를 최대 인덱스로 설정하고 버튼의 상태를 업데이트 함.
		CurrentIndex = NavIndex - 1;

		UpdateButtonSlate();
	}
}

// 요괴 견문록 나가기 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnCreatureExitButtonClick()
{
	// 현재 위젯 모드가 세부 내용 확인 모드인 경우
	if (HintState == EHintStates::AS_Detail)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 뒤로 가기 버튼을 보이게 하고, 요괴 견문록 패널을 안보이게 함.
		Exit->SetVisibility(ESlateVisibility::Visible);
		CreaturePanel->SetVisibility(ESlateVisibility::Collapsed);
		
		// 그 후 위젯의 모드를 문서 확인 모드로 설정함.
		HintState = EHintStates::AS_Search;
	}
}

// 도움말 나가기 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnHintExitButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 뒤로 가기 버튼을 보이게 하고, 도움말 패널을 안 보이게 설정함.
		Exit->SetVisibility(ESlateVisibility::Visible);
		HintPanel->SetVisibility(ESlateVisibility::Collapsed);

		// 그 후 위젯의 모드를 문서 확인 모드로 설정함.
		HintState = EHintStates::AS_Search;
	}
}

// 1번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive1ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 리퍼에게 죽은 경험이 있다면 리퍼 데이터를 가져와 도감을 생성하고 출력
		if (CatchedByReaper)
		{
			// 요괴 데이터에서 리퍼의 정보를 가져옴.
			FCreatureData* ReaperData = HintData->FindRow<FCreatureData>(*FString::FromInt(1), TEXT("")); // 리퍼 데이터

			// 위젯 모드를 세부 내용 확인 모드로 설정함.
			HintState = EHintStates::AS_Detail;

			// 견문록 패널의 텍스트와 이미지에 가져온 정보를 입력함.
			CreatureName->SetText(ReaperData->Title);
			CreatureImage->SetBrushFromTexture(ReaperData->Image);
			Detail->SetText(ReaperData->Detail);
			CreaturePanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 1번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive1ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 리퍼에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CatchedByReaper)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Reaper", "Document of the Grim Reaper"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}

		// 현재 버튼 인덱스를 0으로 설정하고 버튼의 상태를 업데이트 함.
		CurrentIndex = 0;

		UpdateButtonSlate();
	}
}

// 2번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive2ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		
		// 러너에게 죽은 경험이 있다면 러너 데이터를 가져와 도감을 생성하고 출력
		if (CatchedByRunner) 
		{
			// 요괴 데이터에서 러너의 정보를 가져옴.
			FCreatureData* RunnerData = HintData->FindRow<FCreatureData>(*FString::FromInt(2), TEXT("")); // 러너 데이터

			// 위젯 모드를 세부 내용 확인 모드로 설정함.
			HintState = EHintStates::AS_Detail;

			// 견문록 패널의 텍스트와 이미지에 가져온 정보를 입력시킴.
			CreatureName->SetText(RunnerData->Title);
			CreatureImage->SetBrushFromTexture(RunnerData->Image);
			Detail->SetText(RunnerData->Detail);
			CreaturePanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 2번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive2ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 러너에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CatchedByRunner)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Runner", "Document of the Runner"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		
		// 현재 버튼 인덱스를 1로 설정하고, 버튼의 상태를 업데이트 함.
		CurrentIndex = 1;

		UpdateButtonSlate();
	}
}

// 3번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive3ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 브루트에게 죽은 경험이 있다면 브루트 데이터를 가져와 도감을 생성하고 출력
		if (CatchedByBrute) 
		{
			// 요괴 데이터에서 브루트의 정보를 가져옴.
			FCreatureData* BruteData = HintData->FindRow<FCreatureData>(*FString::FromInt(3), TEXT("")); // 브루트 데이터

			// 현재 위젯의 모드를 세부 내용 확인 모드로 설정함.
			HintState = EHintStates::AS_Detail;

			// 견문록 패널의 텍스트와 이미지에 가져온 정보를 입력함.
			CreatureName->SetText(BruteData->Title);
			CreatureImage->SetBrushFromTexture(BruteData->Image);
			Detail->SetText(BruteData->Detail);
			CreaturePanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 3번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive3ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 브루트에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CatchedByBrute)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Brute", "Document of the Brute"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}

		// 현재 버튼 인덱스를 2로 설정하고, 버튼의 상태를 업데이트 함.
		CurrentIndex = 2;

		UpdateButtonSlate();
	}
}

// 4번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive4ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		
		// 그슨대에게 죽은 경험이 있다면 그슨대 데이터를 가져와 도감을 생성하고 출력
		if (CatchedByShadow) 
		{
			// 요괴 데이터에서 그슨대의 정보를 가져옴.
			FCreatureData* ShadowData = HintData->FindRow<FCreatureData>(*FString::FromInt(4), TEXT("")); // 그슨대 데이터

			// 현재 위젯의 모드를 세부 내역 확인 모드로 변경함.
			HintState = EHintStates::AS_Detail;

			// 견문록 패널의 텍스트와 이미지에 가져온 정보를 입력함.
			CreatureName->SetText(ShadowData->Title);
			CreatureImage->SetBrushFromTexture(ShadowData->Image);
			Detail->SetText(ShadowData->Detail);
			CreaturePanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 4번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive4ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 그슨대에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CatchedByShadow)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Shadow", "Document of the Shadow"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}

		// 현재 버튼 인덱스를 3으로 설정하고 버튼의 상태를 업데이트 함.
		CurrentIndex = 3;

		UpdateButtonSlate();
	}
}

// 5번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive5ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}

		// 요괴의 특징을 보았다면 특징 데이터를 가져와 도감을 생성하고 출력
		if (SeeCharacteristic) 
		{
			// 요괴 데이터에서 특징의 정보를 가져옴.
			FCreatureData* CharacteristicData = HintData->FindRow<FCreatureData>(*FString::FromInt(5), TEXT("")); // 특징 데이터

			// 현재 위젯의 모드를 세부 내역 확인 모드로 변경함.
			HintState = EHintStates::AS_Detail;

			// 도움말 패널의 텍스트에 가져온 정보를 입력함.
			HintText->SetText(CharacteristicData->Detail);
			HintPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 5번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive5ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 이동 음을 재생함.
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 요괴의 특징을 보았다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (SeeCharacteristic)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip1", "Tip of the creature's characteristics"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}

		// 현재 버튼 인덱스를 4로 설정하고 버튼의 상태를 업데이트 함.
		CurrentIndex = 4;

		UpdateButtonSlate();
	}
}

// 6번 문서 버튼을 클릭했을 때 작동할 함수.
void UHintWidget::OnArchive6ButtonClick()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		// 버튼 클릭 음을 재생함.
		if (IsValid(ButtonClickSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
		}
		
		// 회피 방법을 보았다면 회피 방법 데이터를 가져와 도감을 생성하고 출력
		if (SeeHowToEscape)
		{
			// 요괴 데이터에서 회피 방법 정보를 가져옴.
			FCreatureData* EscapeData = HintData->FindRow<FCreatureData>(*FString::FromInt(6), TEXT("")); // 회피 방법 데이터

			// 현재 위젯의 모드를 세부 내역 확인 모드로 변경함.
			HintState = EHintStates::AS_Detail;

			// 도움말 패널의 텍스트에 가져온 정보를 입력함.
			HintText->SetText(EscapeData->Detail);
			HintPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 6번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UHintWidget::OnArchive6ButtonHover()
{
	// 현재 위젯 모드가 문서 항목 확인 모드인 경우
	if (HintState == EHintStates::AS_Search)
	{
		if (IsValid(ButtonMoveSound))
		{
			UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
		}

		// 회피 방법을 보았다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (SeeHowToEscape)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip2", "Tip on how to escape the creatures"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}

		// 현재 버튼 인덱스를 5로 설정하고 버튼의 상태를 업데이트 함.
		CurrentIndex = 5;

		UpdateButtonSlate();
	}
}

// 버튼의 상태를 업데이트하는 함수.
void UHintWidget::UpdateButtonSlate()
{
	switch (CurrentIndex)
	{
		// 현재 버튼 인덱스가 0일 때 1번 문서 버튼만 흰색으로 설정함.
		case 0:
		{
			Archive1->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 리퍼에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (CatchedByReaper)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Reaper", "Document of the Grim Reaper"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}
			return;
		}
		// 현재 버튼 인덱스가 1일 때 2번 문서 버튼만 흰색으로 설정함.
		case 1:
		{
			Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 러너에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (CatchedByRunner)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Runner", "Document of the Runner"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}
			return;
		}
		// 현재 버튼 인덱스가 2일 때 3번 문서 버튼만 흰색으로 설정함.
		case 2:
			{Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 브루트에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (CatchedByBrute)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Brute", "Document of the Brute"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}
			return;
		}
		// 현재 버튼 인덱스가 3일 때 4번 문서 버튼만 흰색으로 설정함.
		case 3:
		{
			Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 그슨대에게 죽은 경험이 있다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (CatchedByShadow)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Shadow", "Document of the Shadow"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}
			return;
		}
		// 현재 버튼 인덱스가 4일 때 5번 문서 버튼만 흰색으로 설정함.
		case 4:
		{	
			Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 요괴 특징을 보았다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (SeeCharacteristic)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip1", "Tip of the creature's characteristics"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}
			return;
		}
		// 현재 버튼 인덱스가 5일 때 6번 문서 버튼만 흰색으로 설정함.
		case 5:
		{
			Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

			// 회피 방법을 보았다면 기본 설명란에 문구를 추가하고 아니면 비워둠.
			if (SeeHowToEscape)
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip2", "Tip on how to escape the creatures"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			}

			return;
		}
		// 현재 버튼 인덱스가 6일 때 뒤로 가기 버튼만 흰색으로 설정함.
		case 6:
		{
			Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
			Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

			// 설명란은 비워둠.
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

			return;
		}
	}
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UHintWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		// 현재 위젯의 모드가 문서 항목 모드라면
		if (HintState == EHintStates::AS_Search)
		{
			switch (CurrentIndex)
			{
				// 현재 버튼 인덱스가 0이면 1번 문서 버튼을 클릭한 효과를 줌.
				case 0:
				{
					OnArchive1ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 1이면 2번 문서 버튼을 클릭한 효과를 줌.
				case 1:
				{
					OnArchive2ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 2이면 3번 문서 버튼을 클릭한 효과를 줌.
				case 2:
				{
					OnArchive3ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 3이면 4번 문서 버튼을 클릭한 효과를 줌.
				case 3:
				{
					OnArchive4ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 4이면 5번 문서 버튼을 클릭한 효과를 줌.
				case 4:
				{
					OnArchive5ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 5이면 6번 문서 버튼을 클릭한 효과를 줌.
				case 5:
				{
					OnArchive6ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 6이면 뒤로 가기 버튼을 클릭한 효과를 줌.
				case 6:
				{
					OnExitButtonClick();
					break;
				}
			}
		}
		// 현재 위젯 모드가 세부 내용 확인 모드라면
		else if (HintState == EHintStates::AS_Detail)
		{
			// 힌트 패널이 출력되고 있는 경우, 힌트 패널 나가기 버튼을 클릭한 효과를 줌.
			if (HintPanel->GetVisibility() == ESlateVisibility::Visible)
			{
				OnHintExitButtonClick();
			}
			// 견문록 패널이 출력되고 있는 경우, 견문록 패널 나가기 버튼을 클릭한 효과를 줌.
			else if (CreaturePanel->GetVisibility() == ESlateVisibility::Visible)
			{
				OnCreatureExitButtonClick();
			}
		}
	}
	// 엔터 키가 아닌 경우
	else
	{
		// D 키이거나 우측 화살표 키인경우
		if (KeyType == "D" || KeyType == "Right")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 증가시키고 최대 인덱스를 넘어갈 경우 0으로 설정함.
			CurrentIndex++;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex = 0;
			}
		}
		// A 키이거나 좌측 화살표 키인경우
		else if (KeyType == "A" || KeyType == "Left")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 감소시키고 0 미만일 경우 최대 인덱스로 설정함.
			CurrentIndex--;
			if (CurrentIndex < 0)
			{
				CurrentIndex = NavIndex - 1;
			}
		}
		// S 키이거나 아래 화살표 키인경우
		if (KeyType == "S" || KeyType == "Down")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 2 증가시키고 최대 인덱스를 넘어갈 경우 최대 인덱스만큼 빼줌.
			CurrentIndex += 2;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex -= NavIndex;
			}
		}
		// W 키이거나 위 화살표 키인경우
		else if (KeyType == "W" || KeyType == "Up")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 2 감소시키고 0 미만일 경우 최대 인덱스만큼 더함.
			CurrentIndex -= 2;
			if (CurrentIndex < 0)
			{
				CurrentIndex += NavIndex;
			}
		}
		
		// 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}