// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/HintWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "HUD/ArchiveWidget.h"
#include "Sound/SoundCue.h"


void UHintWidget::NativeConstruct()
{
	Super::NativeConstruct();

	this->SetKeyboardFocus();

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

	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{

		if (SaveData->CollectArchives.CatchedByReaper)
		{
			CatchedByReaper = true;
			Archive1Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Reaper", "Grim Reaper"));
		}

		if (SaveData->CollectArchives.CatchedByRunner)
		{
			CatchedByRunner = true;
			Archive2Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Runner", "Runner"));
		}

		if (SaveData->CollectArchives.CatchedByBrute)
		{
			CatchedByBrute = true;
			Archive3Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Brute", "Brute"));
		}

		if (SaveData->CollectArchives.CatchedByShadow)
		{
			CatchedByShadow = true;
			Archive4Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Shadow", "Shadow"));
		}

		if (SaveData->CollectArchives.SeeCharacteristic)
		{
			SeeCharacteristic = true;
			Archive5Text->SetText(NSLOCTEXT("UHintWidget", "Archive_Characteristic", "Creature Characteristic"));
		}

		if (SaveData->CollectArchives.SeeHowToEscape)
		{
			SeeHowToEscape = true;
			Archive6Text->SetText(NSLOCTEXT("UHintWidget", "Archive_HowToEscape", "How to escape"));
		}
	}

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

void UHintWidget::OnExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	UArchiveWidget* FromWidget = CreateWidget<UArchiveWidget>(GetWorld(), ParentWidget);
	FromWidget->AddToViewport();
	//FromWidget->bIsFocusable = true;
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

void UHintWidget::OnExitButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = NavIndex - 1;

	UpdateButtonSlate();
}

void UHintWidget::OnCreatureExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	Exit->SetVisibility(ESlateVisibility::Visible);
	CreaturePanel->SetVisibility(ESlateVisibility::Collapsed);
	HintState = EHintStates::AS_Search;
}

void UHintWidget::OnHintExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	Exit->SetVisibility(ESlateVisibility::Visible);
	HintPanel->SetVisibility(ESlateVisibility::Collapsed);
	HintState = EHintStates::AS_Search;
}

void UHintWidget::OnArchive1ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (CatchedByReaper) // 리퍼에게 죽은 경험이 있다면 리퍼 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* ReaperData = HintData->FindRow<FCreatureData>(*FString::FromInt(1), TEXT("")); // 리퍼 데이터

		HintState = EHintStates::AS_Detail;

		//CreatureName->SetText(FText::FromString(ReaperData->Title));
		CreatureName->SetText(ReaperData->Title);
		CreatureImage->SetBrushFromTexture(ReaperData->Image);
		//Detail->SetText(FText::FromString(ReaperData->Detail));
		Detail->SetText(ReaperData->Detail);
		CreaturePanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive1ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (CatchedByReaper)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Reaper", "Document of the Grim Reaper"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 0;

	UpdateButtonSlate();
}

void UHintWidget::OnArchive2ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	
	if (CatchedByRunner) // 러너에게 죽은 경험이 있다면 러너 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* RunnerData = HintData->FindRow<FCreatureData>(*FString::FromInt(2), TEXT("")); // 러너 데이터

		HintState = EHintStates::AS_Detail;

		//CreatureName->SetText(FText::FromString(RunnerData->Title));
		CreatureName->SetText(RunnerData->Title);
		CreatureImage->SetBrushFromTexture(RunnerData->Image);
		//Detail->SetText(FText::FromString(RunnerData->Detail));
		Detail->SetText(RunnerData->Detail);
		CreaturePanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive2ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (CatchedByRunner)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Runner", "Document of the Runner"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 1;

	UpdateButtonSlate();
}

void UHintWidget::OnArchive3ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (CatchedByBrute) // 브루트에게 죽은 경험이 있다면 브루트 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* BruteData = HintData->FindRow<FCreatureData>(*FString::FromInt(3), TEXT("")); // 브루트 데이터

		HintState = EHintStates::AS_Detail;

		//CreatureName->SetText(FText::FromString(BruteData->Title));
		CreatureName->SetText(BruteData->Title);
		CreatureImage->SetBrushFromTexture(BruteData->Image);
		//Detail->SetText(FText::FromString(BruteData->Detail));
		Detail->SetText(BruteData->Detail);
		CreaturePanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive3ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (CatchedByBrute)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Brute", "Document of the Brute"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 2;

	UpdateButtonSlate();
}

void UHintWidget::OnArchive4ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (CatchedByShadow) // 그슨대에게 죽은 경험이 있다면 그슨대 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* ShadowData = HintData->FindRow<FCreatureData>(*FString::FromInt(4), TEXT("")); // 그슨대 데이터

		HintState = EHintStates::AS_Detail;

		//CreatureName->SetText(FText::FromString(ShadowData->Title));
		CreatureName->SetText(ShadowData->Title);
		CreatureImage->SetBrushFromTexture(ShadowData->Image);
		//Detail->SetText(FText::FromString(ShadowData->Detail));
		Detail->SetText(ShadowData->Detail);	
		CreaturePanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive4ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (CatchedByShadow)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Shadow", "Document of the Shadow"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 3;

	UpdateButtonSlate();
}

void UHintWidget::OnArchive5ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (SeeCharacteristic) // 크리쳐의 특징을 보았다면 특징 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* CharacteristicData = HintData->FindRow<FCreatureData>(*FString::FromInt(5), TEXT("")); // 특징 데이터

		HintState = EHintStates::AS_Detail;

		//HintText->SetText(FText::FromString(CharacteristicData->Detail));
		HintText->SetText(CharacteristicData->Detail);
		HintPanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive5ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (SeeCharacteristic)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip1", "Tip of the creature's characteristics"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 4;

	UpdateButtonSlate();
}

void UHintWidget::OnArchive6ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (SeeHowToEscape) // 회피 방법을 보았다면 회피 방법 데이터를 가져와 도감을 생성하고 출력
	{
		FCreatureData* EscapeData = HintData->FindRow<FCreatureData>(*FString::FromInt(6), TEXT("")); // 회피 방법 데이터

		HintState = EHintStates::AS_Detail;

		//HintText->SetText(FText::FromString(EscapeData->Detail));
		HintText->SetText(EscapeData->Detail);
		HintPanel->SetVisibility(ESlateVisibility::Visible);
		Exit->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UHintWidget::OnArchive6ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (SeeHowToEscape)
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip2", "Tip on how to escape the creatures"));
	}
	else
	{
		ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

	}
	CurrentIndex = 5;

	UpdateButtonSlate();
}

void UHintWidget::UpdateButtonSlate()
{
	switch (CurrentIndex)
	{
	case 0:
		Archive1->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (CatchedByReaper)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Reaper", "Document of the Grim Reaper"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 1:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (CatchedByRunner)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Runner", "Document of the Runner"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 2:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (CatchedByBrute)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Brute", "Document of the Brute"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 3:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (CatchedByShadow)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Shadow", "Document of the Shadow"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 4:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (SeeCharacteristic)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip1", "Tip of the creature's characteristics"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 5:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (SeeHowToEscape)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_Tip2", "Tip on how to escape the creatures"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
		break;
	case 6:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

		if (SeeHowToEscape)
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));
		}
		break;
	}
}

FReply UHintWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		if (HintState == EHintStates::AS_Search) // 현재 문서 항목 페이지라면
		{
			switch (CurrentIndex)
			{
			case 0:
				OnArchive1ButtonClick();
				break;
			case 1:
				OnArchive2ButtonClick();
				break;
			case 2:
				OnArchive3ButtonClick();
				break;
			case 3:
				OnArchive4ButtonClick();
				break;
			case 4:
				OnArchive5ButtonClick();
				break;
			case 5:
				OnArchive6ButtonClick();
				break;
			case 6:
				OnExitButtonClick();
				break;
			}
		}
		else if (HintState == EHintStates::AS_Detail)// 아이템 내용 확인 페이지라면
		{
			if (HintPanel->GetVisibility() == ESlateVisibility::Visible)
			{
				OnHintExitButtonClick();
			}
			else if (CreaturePanel->GetVisibility() == ESlateVisibility::Visible)
			{
				OnCreatureExitButtonClick();
			}
		}
	}
	else
	{
		if (KeyType == "D" || KeyType == "Right")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			CurrentIndex++;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex = 0;
			}
		}
		else if (KeyType == "A" || KeyType == "Left")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			CurrentIndex--;
			if (CurrentIndex < 0)
			{
				CurrentIndex = NavIndex - 1;
			}
		}
		if (KeyType == "S" || KeyType == "Down")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			CurrentIndex += 2;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex -= NavIndex;
			}
		}
		else if (KeyType == "W" || KeyType == "Up")
		{
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			CurrentIndex -= 2;
			if (CurrentIndex < 0)
			{
				CurrentIndex += NavIndex;
			}
		}
		UpdateButtonSlate();
	}

	return reply;
}