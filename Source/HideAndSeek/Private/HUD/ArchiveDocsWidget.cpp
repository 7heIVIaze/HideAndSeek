// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/ArchiveDocsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "HUD/ArchiveWidget.h"
#include "Sound/SoundCue.h"

void UArchiveDocsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(CreateAnim);

	this->SetKeyboardFocus();

	EndDelegate.BindDynamic(this, &UArchiveDocsWidget::AnimationFinished);
	BindToAnimationFinished(RemoveAnim, EndDelegate);
	
	Item1_CigarLighter = false;
	Item2_FlashLight = false;
	Item3_Key = false;
	Item4_Timer = false;
	Item5_BronzeSword = false;
	Item6_BronzeBell = false;
	Item7_BronzeMirror = false;
	Item8_Extinguisher = false;
	Item9_Cutter = false;
	Item10_SoulLantern = false;

	Article1 = false;
	Article2 = false;
	Article3 = false;
	Article4 = false;
	Article5 = false;
	
	Letter1_Berith1 = false;
	Letter2_Berith2 = false;
	Letter3_Berith3 = false;
	Letter4_Berith4 = false;
	Letter5_Berith5 = false;
	Letter6_Berith6 = false;
	Letter7_Gamigin1 = false;
	Letter8_Gamigin2 = false;
	Letter9_Gamigin3 = false;
	Letter10_Gamigin4 = false;

	Diary1 = false;
	Diary2 = false;
	Diary3 = false;
	Diary4 = false;
	Diary5 = false;

	Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive9Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	
	CurrentIndex = 0;
	ArchiveState = EArchiveStates::AS_Search;
	ItemPanel->SetVisibility(ESlateVisibility::Collapsed);
	DocumentPanel->SetVisibility(ESlateVisibility::Collapsed);

	Article->SetVisibility(ESlateVisibility::Collapsed);
	Letter->SetVisibility(ESlateVisibility::Collapsed);

	if (UHorrorGameSaveGame* SaveData = UHorrorGameSaveGame::LoadObject(this, TEXT("Player"), 0))
	{
		if (ArchiveMode == EArchiveModes::AM_Item)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Archive Mode: AM_Item")));
			NavIndex = 11;

			if (SaveData->Item1_CigarLighter)
			{
				Item1_CigarLighter = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_CigarLighter", "CigarLighter"));
			}

			if (SaveData->Item2_FlashLight)
			{
				Item2_FlashLight = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_FlashLight", "FlashLight"));
			}

			if (SaveData->Item3_Key)
			{
				Item3_Key = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Key", "Key"));
			}

			if (SaveData->Item4_Timer)
			{
				Item4_Timer = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Timer", "Timer"));
			}
			
			if (SaveData->Item5_BronzeSword)
			{
				Item5_BronzeSword = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeSword", "Bronze Sword"));
			}

			if (SaveData->Item6_BronzeBell)
			{
				Item6_BronzeBell = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeBell", "Bronze Bell"));
			}

			if (SaveData->Item7_BronzeMirror)
			{
				Item7_BronzeMirror = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeMirror", "Bronze Mirror"));
			}

			if (SaveData->Item8_Extinguisher)
			{
				Item8_Extinguisher = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Extinguisher", "Extinguisher"));
			}

			if (SaveData->Item9_Cutter)
			{
				Item9_Cutter = true;
				Archive9Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Cutter", "Cutter"));
			}

			if (SaveData->Item10_SoulLantern)
			{
				Item10_SoulLantern = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_SoulLantern", "Soul Lantern"));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Archive Mode: AM_Document")));
			NavIndex = 11;

			if (SaveData->Article1)
			{
				Article1 = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article1", "1/4 Article"));
			}

			if (SaveData->Article2)
			{
				Article2 = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article2", "12/7 Article"));
			}

			if (SaveData->Article3)
			{
				Article3 = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article3", "12/18 Article"));
			}

			if (SaveData->Article4)
			{
				Article4 = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article4", "1/11 Article"));
			}

			if (SaveData->Article5)
			{
				Article5 = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article5", "2/6 Article"));
			}

			if (SaveData->Diary1)
			{
				Diary1 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary1", "11/4 Diary"));
			}

			if (SaveData->Diary2)
			{
				Diary2 = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary2", "Summon's Diary"));
			}

			if (SaveData->Diary3)
			{
				Diary3 = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary3", "1/3 Diary"));
			}

			if (SaveData->Diary4)
			{
				Diary4 = true;
				Archive9Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary4", "12/17 Diary"));
			}

			if (SaveData->Diary5)
			{
				Diary5 = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary5", "2/6 Diary"));
			}

		}
		else if (ArchiveMode == EArchiveModes::AM_Letter)
		{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Archive Mode: AM_Letter")));
			NavIndex = 11;

			if (SaveData->Letter1_Berith1)
			{
				Letter1_Berith1 = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter1", "Berith's Letter 1"));
			}

			if (SaveData->Letter2_Berith2)
			{
				Letter2_Berith2 = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter2", "Berith's Letter 2"));
			}

			if (SaveData->Letter3_Berith3)
			{
				Letter3_Berith3 = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter3", "Berith's Letter 3"));
			}

			if (SaveData->Letter4_Berith4)
			{
				Letter4_Berith4 = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter4", "Berith's Letter 4"));
			}

			if (SaveData->Letter5_Berith5)
			{
				Letter5_Berith5 = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter5", "Berith's Letter 5"));
			}

			if (SaveData->Letter6_Berith6)
			{
				Letter6_Berith6 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter6", "Berith's Letter 6"));
			}

			if (SaveData->Letter7_Gamigin1)
			{
				Letter7_Gamigin1 = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter7", "Gamigin's Letter 1"));
			}

			if (SaveData->Letter8_Gamigin2)
			{
				Letter8_Gamigin2 = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter8", "Gamigin's Letter 2"));
			}

			if (SaveData->Letter9_Gamigin3)
			{
				Letter9_Gamigin3 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter9", "Gamigin's Letter 3"));
			}

			if (SaveData->Letter10_Gamigin4)
			{
				Letter10_Gamigin4 = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter10", "Gamigin's Letter 4"));
			}
		}

		Archive1->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive1ButtonClick);
		Archive1->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive1ButtonHover);

		Archive2->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive2ButtonClick);
		Archive2->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive2ButtonHover);

		Archive3->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive3ButtonClick);
		Archive3->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive3ButtonHover);

		Archive4->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive4ButtonClick);
		Archive4->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive4ButtonHover);

		Archive5->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive5ButtonClick);
		Archive5->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive5ButtonHover);

		Archive6->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive6ButtonClick);
		Archive6->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive6ButtonHover);

		Archive7->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive7ButtonClick);
		Archive7->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive7ButtonHover);

		Archive8->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive8ButtonClick);
		Archive8->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive8ButtonHover);

		Archive9->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive9ButtonClick);
		Archive9->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive9ButtonHover);

		Archive10->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive10ButtonClick);
		Archive10->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive10ButtonHover);

		Exit->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnExitButtonClick);
		Exit->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnExitButtonHover);

		ItemExit->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnItemExitButtonClick);
		DocumentExit->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnDocumentExitButtonClick);
	}

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnExitButtonClick()
{
	ParentWidget->bIsFocusable = true;
	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ParentWidget->SetUserFocus(Controller);
	ParentWidget->SetKeyboardFocus();
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	RemoveFromParent();
}

void UArchiveDocsWidget::OnExitButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = NavIndex - 1;
	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnItemExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	Exit->SetVisibility(ESlateVisibility::Visible);
	ItemPanel->SetVisibility(ESlateVisibility::Collapsed);
	ArchiveState = EArchiveStates::AS_Search;
}

void UArchiveDocsWidget::OnDocumentExitButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	Exit->SetVisibility(ESlateVisibility::Visible);
	DocumentPanel->SetVisibility(ESlateVisibility::Collapsed);
	Article->SetVisibility(ESlateVisibility::Collapsed);
	Letter->SetVisibility(ESlateVisibility::Collapsed);
	ArchiveState = EArchiveStates::AS_Search;
}

void UArchiveDocsWidget::OnArchive1ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item1_CigarLighter) // 라이터를 한 번이라도 습득해놓은 상태면 확인할 수 있도록
		{
			FHorrorGameItemData* CigarLightData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(1), TEXT("")); // 라이터 데이터
			//FString ItemName = CigarLightData->ItemName;
			FText ItemName = CigarLightData->ItemName;
			UTexture2D* Texture = CigarLightData->ItemIcon;
			//FString ItemDetail = CigarLightData->ItemDetail;
			FText ItemDetail = CigarLightData->ItemDetail;
			
			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Article1)
		{
			FCollectArchivesData* Article1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(1), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Article1Data->Detail_EN));
			Document->SetText(Article1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Article1Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter1_Berith1)
		{
			FCollectArchivesData* Letter1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(6), TEXT("")); // 1번 기사 데이터
			
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter1Data->Detail_EN));
			Document->SetText(Letter1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter1Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive1ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item1_CigarLighter) // 라이터를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_CigarLight", "Description of the lighter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Article1) // 기사 1을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article1", "Article dated January 4th obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter1_Berith1) // 베리스의 편지 1을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter1", "First letter from Berith obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 0;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive2ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item2_FlashLight) // 라이터를 한 번이라도 습득해놓은 상태면 확인할 수 있도록
		{
			FHorrorGameItemData* FlashLightData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(2), TEXT("")); // 라이터 데이터
			//FString ItemName = FlashLightData->ItemName;
			FText ItemName = FlashLightData->ItemName;
			UTexture2D* Texture = FlashLightData->ItemIcon;
			//FString ItemDetail = FlashLightData->ItemDetail;
			FText ItemDetail = FlashLightData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Article2)
		{
			FCollectArchivesData* Article2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(2), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Article2Data->Detail_EN));
			Document->SetText(Article2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Article2Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter2_Berith2)
		{
			FCollectArchivesData* Letter2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(7), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter2Data->Detail_EN));
			Document->SetText(Letter2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter2Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive2ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item2_FlashLight) // 플래시라이트를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Flashlight", "Description of the flashlight"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Article2) // 기사 2을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article2", "Article dated December 7th obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter2_Berith2) // 베리스의 편지 2을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter2", "Second letter from Berith obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 1;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive3ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item3_Key)
		{
			FHorrorGameItemData* KeyData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(3), TEXT("")); // 라이터 데이터
			//FString ItemName = KeyData->ItemName;
			FText ItemName = KeyData->ItemName;
			UTexture2D* Texture = KeyData->ItemIcon;
			//FString ItemDetail = KeyData->ItemDetail;
			FText ItemDetail = KeyData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Article3)
		{
			FCollectArchivesData* Article3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(3), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Article3Data->Detail_EN));
			Document->SetText(Article3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Article3Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter3_Berith3)
		{
			FCollectArchivesData* Letter3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(8), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter3Data->Detail_EN));
			Document->SetText(Letter3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter3Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive3ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item3_Key) // 열쇠를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Key", "Description of the key"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Article3) // 기사 3을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article3", "Article dated December 18th obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter3_Berith3) // 베리스의 편지 3을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter3", "Third letter from Berith obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 2;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive4ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item4_Timer)
		{
			FHorrorGameItemData* TimerData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(4), TEXT("")); // 라이터 데이터
			//FString ItemName = TimerData->ItemName;
			FText ItemName = TimerData->ItemName;
			UTexture2D* Texture = TimerData->ItemIcon;
			//FString ItemDetail = TimerData->ItemDetail;
			FText ItemDetail = TimerData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Article4)
		{
			FCollectArchivesData* Article4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(4), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Article4Data->Detail_EN));
			Document->SetText(Article4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Article4Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter4_Berith4)
		{
			FCollectArchivesData* Letter4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(9), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter4Data->Detail_EN));
			Document->SetText(Letter4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter4Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive4ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item4_Timer) // 타이머를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Timer", "Description of the timer"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Article4) // 기사 4을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article4", "Article dated January 11th obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter4_Berith4) // 베리스의 편지 4을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter4", "Fourth letter from Berith obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 3;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive5ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item6_BronzeBell)
		{
			FHorrorGameItemData* BronzeSwordData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(5), TEXT("")); // 라이터 데이터
			//FString ItemName = BronzeSwordData->ItemName;
			FText ItemName = BronzeSwordData->ItemName;
			UTexture2D* Texture = BronzeSwordData->ItemIcon;
			//FString ItemDetail = BronzeSwordData->ItemDetail;
			FText ItemDetail = BronzeSwordData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			//Title->SetText(FText::FromString(ItemName));
			Detail->SetText(ItemDetail);
			//Detail->SetText(FText::FromString(ItemDetail));
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Article5)
		{
			FCollectArchivesData* Article5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(5), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Article5Data->Detail_EN));
			Document->SetText(Article5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Article5Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter5_Berith5)
		{
			FCollectArchivesData* Letter5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(10), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter5Data->Detail_EN));
			Document->SetText(Letter5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter5Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive5ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item5_BronzeSword) // 청동 검을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Sword", "Description of the bronze sword"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Article5) // 기사 5을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article5", "Article dated February 6th obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter5_Berith5) // 베리스의 편지 5을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter5", "Fifth letter from Berith obtained from 'The True Color' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 4;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive6ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item6_BronzeBell)
		{
			FHorrorGameItemData* BronzeBellData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(6), TEXT("")); // 라이터 데이터
			//FString ItemName = BronzeBellData->ItemName;
			FText ItemName = BronzeBellData->ItemName;
			UTexture2D* Texture = BronzeBellData->ItemIcon;
			//FString ItemDetail = BronzeBellData->ItemDetail;
			FText ItemDetail = BronzeBellData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Diary1)
		{
			FCollectArchivesData* Diary1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(16), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Diary1Data->Detail_EN));
			Document->SetText(Diary1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Diary1Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter6_Berith6)
		{
			FCollectArchivesData* Letter6Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(11), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter6Data->Detail_EN));
			Document->SetText(Letter6Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter6Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive6ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item6_BronzeBell) // 청동 방울을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Bell", "Description of the bronze bell"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Diary1) // 일기 1을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary1", "Diary dated November 4th obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter6_Berith6) // 베리스의 편지 6을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter6", "Sixth letter from Berith obtained from 'The True Color' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 5;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive7ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item7_BronzeMirror)
		{
			FHorrorGameItemData* BronzeMirrorData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(7), TEXT("")); // 라이터 데이터
			//FString ItemName = BronzeMirrorData->ItemName;
			FText ItemName = BronzeMirrorData->ItemName;
			UTexture2D* Texture = BronzeMirrorData->ItemIcon;
			//FString ItemDetail = BronzeMirrorData->ItemDetail;
			FText ItemDetail = BronzeMirrorData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Diary2)
		{
			FCollectArchivesData* Diary2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(17), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Diary2Data->Detail_EN));
			Document->SetText(Diary2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Diary2Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter7_Gamigin1)
		{
			FCollectArchivesData* Letter7Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(12), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter7Data->Detail_EN));
			Document->SetText(Letter7Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter7Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive7ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item7_BronzeMirror) // 청동 거울을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Mirror", "Description of the bronze mirror"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Diary2) // 일기 2을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary2", "Someone's diary obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter7_Gamigin1) // 가미긴의 편지 1을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter7", "First letter from Gamigin obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 6;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive8ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item8_Extinguisher)
		{
			FHorrorGameItemData* ExtinguisherData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(8), TEXT("")); // 라이터 데이터
			//FString ItemName = ExtinguisherData->ItemName;
			FText ItemName = ExtinguisherData->ItemName;
			UTexture2D* Texture = ExtinguisherData->ItemIcon;
			//FString ItemDetail = ExtinguisherData->ItemDetail;
			FText ItemDetail = ExtinguisherData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Diary3)
		{
			FCollectArchivesData* Diary3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(18), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Diary3Data->Detail_EN));
			Document->SetText(Diary3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Diary3Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter8_Gamigin2)
		{
			FCollectArchivesData* Letter8Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(13), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter8Data->Detail_EN));
			Document->SetText(Letter8Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter8Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive8ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item8_Extinguisher) // 소화기를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Extinguisher", "Description of the extinguisher"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Diary3) // 일기 3을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary3", "Diary dated January 3rd obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter8_Gamigin2) // 가미긴의 편지 2을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter8", "Second letter from Gamigin obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 7;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive9ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item9_Cutter)
		{
			FHorrorGameItemData* CutterData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(9), TEXT("")); // 라이터 데이터
			//FString ItemName = CutterData->ItemName;
			FText ItemName = CutterData->ItemName;
			UTexture2D* Texture = CutterData->ItemIcon;
			//FString ItemDetail = CutterData->ItemDetail;
			FText ItemDetail = CutterData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Diary4)
		{
			FCollectArchivesData* Diary4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(19), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Diary4Data->Detail_EN));
			Document->SetText(Diary4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Diary4Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter9_Gamigin3)
		{
			FCollectArchivesData* Letter9Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(14), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter9Data->Detail_EN));
			Document->SetText(Letter9Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter9Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive9ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item9_Cutter) // 절단기를 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Cutter", "Description of the cutter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Diary4) // 일기 4을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary4", "Diary dated December 17th obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter9_Gamigin3) // 가미긴의 편지 3을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter9", "Third letter from Gamigin obtained from 'The Deal' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 8;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::OnArchive10ButtonClick()
{
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	if (ArchiveMode == EArchiveModes::AM_Item) // 현재 아이템 문서 항목 상태라면
	{
		if (Item10_SoulLantern)
		{
			FHorrorGameItemData* SoulLanternData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(10), TEXT("")); // 라이터 데이터
			//FString ItemName = SoulLanternData->ItemName;
			FText ItemName = SoulLanternData->ItemName;
			UTexture2D* Texture = SoulLanternData->ItemIcon;
			//FString ItemDetail = SoulLanternData->ItemDetail;
			FText ItemDetail = SoulLanternData->ItemDetail;

			ArchiveState = EArchiveStates::AS_Detail;

			ItemImage->SetBrushFromTexture(Texture, true);
			//Title->SetText(FText::FromString(ItemName));
			Title->SetText(ItemName);
			//Detail->SetText(FText::FromString(ItemDetail));
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		if (Diary5)
		{
			FCollectArchivesData* Diary5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(20), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Diary5Data->Detail_EN));
			Document->SetText(Diary5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Diary5Data->Detail_EN));
			}*/
		}
	}

	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		if (Letter10_Gamigin4)
		{
			FCollectArchivesData* Letter10Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(15), TEXT("")); // 1번 기사 데이터

			ArchiveState = EArchiveStates::AS_DocumentDetail;

			//Document->SetText(FText::FromString(Letter10Data->Detail_EN));
			Document->SetText(Letter10Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
			/*if (DocumentWidget != nullptr)
			{
				this->bIsFocusable = false;
				UDocumentWidget* NewWidget = CreateWidget<UDocumentWidget>(GetWorld(), DocumentWidget);
				NewWidget->AddToViewport();
				NewWidget->bIsArchiveMode = true;
				NewWidget->SetDocumentText(FText::FromString(Letter10Data->Detail_EN));
			}*/
		}
	}
}

void UArchiveDocsWidget::OnArchive10ButtonHover()
{
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
	{
		if (Item10_SoulLantern) // 영혼 랜턴을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Lantern", "Description of the soul lantern"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
	{
		if (Diary5) // 일기 5을 얻은 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary5", "Diary dated February 6th obtained from 'The True Color' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
	{
		if (Letter10_Gamigin4) // 가미긴의 편지 4을 획득한 상태라면
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter10", "Fourth letter from Gamigin obtained from the 'Mist Estate' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	CurrentIndex = 9;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::AnimationFinished()
{
	RemoveFromParent();
}

void UArchiveDocsWidget::UpdateButtonSlate()
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
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		
		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item1_CigarLighter) // 라이터를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_CigarLight", "Description of the lighter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Article1) // 기사 1을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article1", "Article dated January 4th obtained from the 'Basement' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter1_Berith1) // 베리스의 편지 1을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter1", "First letter from Berith obtained from the 'Basement' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 1:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item2_FlashLight) // 플래시라이트를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Flashlight", "Description of the flashlight"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Article1) // 기사 2을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article2", "Article dated December 7th obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter1_Berith1) // 베리스의 편지 1을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter2", "Second letter from Berith obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 2:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item3_Key) // 열쇠를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Key", "Description of the key"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Article3) // 기사 3을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article3", "Article dated December 18th obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter3_Berith3) // 베리스의 편지 3을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter3", "Third letter from Berith obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 3:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item4_Timer) // 타이머를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Timer", "Description of the timer"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Article4) // 기사 4을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article4", "Article dated January 11th obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter4_Berith4) // 베리스의 편지 4을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter4", "Fourth letter from Berith obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 4:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item5_BronzeSword) // 청동 검을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Sword", "Description of the bronze sword"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Article5) // 기사 5을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article5", "Article dated February 6th obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter5_Berith5) // 베리스의 편지 5을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter5", "Fifth letter from Berith obtained from 'The True Color' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 5:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item6_BronzeBell) // 청동 방울을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Bell", "Description of the bronze bell"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Diary1) // 일기 1을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary1", "Diary dated November 4th obtained from the 'Basement' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter6_Berith6) // 베리스의 편지 6을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter6", "Sixth letter from Berith obtained from 'The True Color' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}

		break;
	case 6:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item7_BronzeMirror) // 청동 거울을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Mirror", "Description of the bronze mirror"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Diary2) // 일기 2을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary2", "Someone's diary obtained from the 'Basement' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter7_Gamigin1) // 가미긴의 편지 1을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter7", "First letter from Gamigin obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 7:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item8_Extinguisher) // 소화기를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Extinguisher", "Description of the extinguisher"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Diary3) // 일기 3을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary3", "Diary dated January 3rd obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter8_Gamigin2) // 가미긴의 편지 2을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter8", "Second letter from Gamigin obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 8:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item9_Cutter) // 절단기를 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Cutter", "Description of the cutter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Diary4) // 일기 4을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary4", "Diary dated December 17th obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter9_Gamigin3) // 가미긴의 편지 3을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter9", "Third letter from Gamigin obtained from 'The Deal' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 9:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

		if (ArchiveMode == EArchiveModes::AM_Item) // 아이템 아카이브인 경우
		{
			if (Item10_SoulLantern) // 영혼 랜턴을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Lantern", "Description of the soul lantern"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Document) // 문서 아카이브인 경우
		{
			if (Diary5) // 일기 5을 얻은 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary5", "Diary dated February 6th obtained from 'The True Color' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter) // 편지 아카이브인 경우
		{
			if (Letter10_Gamigin4) // 가미긴의 편지 4을 획득한 상태라면
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter10", "Fourth letter from Gamigin obtained from the 'Mist Estate' Chapter"));
			}
			else
			{
				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
			}
		}
		break;
	case 10:
		Archive1->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive2->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive3->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive4->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive5->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive6->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive7->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive8->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive9->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Archive10->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
		Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

		ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Exit", ""));
		break;
	}
}

FReply UArchiveDocsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	if (KeyType == "Enter")
	{
		if (ArchiveState == EArchiveStates::AS_Search) // 현재 문서 항목 페이지라면
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
				OnArchive7ButtonClick();
				break;
			case 7:
				OnArchive8ButtonClick();
				break;
			case 8:
				OnArchive9ButtonClick();
				break;
			case 9:
				OnArchive10ButtonClick();
				break;
			case 10:
				OnExitButtonClick();
				break;
			}
		}
		else if(ArchiveState == EArchiveStates::AS_Detail)// 아이템 내용 확인 페이지라면
		{
			OnItemExitButtonClick();
		}
		else if (ArchiveState == EArchiveStates::AS_DocumentDetail)// 문서 내용 확인 페이지라면
		{
			OnDocumentExitButtonClick();
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