// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/ArchiveDocsWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Kismet/GameplayStatics.h"
#include "StartGameMode.h"
#include "ComponentAction/HorrorGameSaveGame.h"
#include "HUD/ArchiveWidget.h"
#include "Sound/SoundCue.h"

// 초기화하는 생성자
void UArchiveDocsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(CreateAnim);

	this->SetKeyboardFocus();

	EndDelegate.BindDynamic(this, &UArchiveDocsWidget::AnimationFinished);
	BindToAnimationFinished(RemoveAnim, EndDelegate);
	
	CollectedData.Article1 = false;
	CollectedData.Article2 = false;
	CollectedData.Article3 = false;
	CollectedData.Article4 = false;
	CollectedData.Article5 = false;

	CollectedData.Item1_CigarLighter = false;
	CollectedData.Item2_FlashLight = false;
	CollectedData.Item3_Key = false;
	CollectedData.Item4_Timer = false;
	CollectedData.Item5_BronzeSword = false;
	CollectedData.Item6_BronzeBell = false;
	CollectedData.Item7_BronzeMirror = false;
	CollectedData.Item8_Extinguisher = false;
	CollectedData.Item9_Cutter = false;
	CollectedData.Item10_SoulLantern = false;
	CollectedData.Item11_GlowStick = false;

	CollectedData.Letter1_Berith1 = false;
	CollectedData.Letter2_Berith2 = false;
	CollectedData.Letter3_Berith3 = false;
	CollectedData.Letter4_Berith4 = false;
	CollectedData.Letter5_Berith5 = false;
	CollectedData.Letter6_Berith6 = false;
	CollectedData.Letter7_Gamigin1 = false;
	CollectedData.Letter8_Gamigin2 = false;
	CollectedData.Letter9_Gamigin3 = false;
	CollectedData.Letter10_Gamigin4 = false;

	CollectedData.Diary1 = false;
	CollectedData.Diary2 = false;
	CollectedData.Diary3 = false;
	CollectedData.Diary4 = false;
	CollectedData.Diary5 = false;

	/*Item1_CigarLighter = false;
	Item2_FlashLight = false;
	Item3_Key = false;
	Item4_Timer = false;
	Item5_BronzeSword = false;
	Item6_BronzeBell = false;
	Item7_BronzeMirror = false;
	Item8_Extinguisher = false;
	Item9_Cutter = false;
	Item10_SoulLantern = false;
	Item11_GlowStick = false;

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
	Diary5 = false;*/

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
	Archive11Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "ArchiveNone", "???"));
	
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
			NavIndex = 12;

			if (SaveData->CollectArchives.Item1_CigarLighter)
			{
				CollectedData.Item1_CigarLighter = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_CigarLighter", "CigarLighter"));
			}

			if (SaveData->CollectArchives.Item2_FlashLight)
			{
				CollectedData.Item2_FlashLight = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_FlashLight", "FlashLight"));
			}

			if (SaveData->CollectArchives.Item3_Key)
			{
				CollectedData.Item3_Key = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Key", "Key"));
			}

			if (SaveData->CollectArchives.Item4_Timer)
			{
				CollectedData.Item4_Timer = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Timer", "Timer"));
			}
			
			if (SaveData->CollectArchives.Item5_BronzeSword)
			{
				CollectedData.Item5_BronzeSword = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeSword", "Bronze Sword"));
			}

			if (SaveData->CollectArchives.Item6_BronzeBell)
			{
				CollectedData.Item6_BronzeBell = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeBell", "Bronze Bell"));
			}

			if (SaveData->CollectArchives.Item7_BronzeMirror)
			{
				CollectedData.Item7_BronzeMirror = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_BronzeMirror", "Bronze Mirror"));
			}

			if (SaveData->CollectArchives.Item8_Extinguisher)
			{
				CollectedData.Item8_Extinguisher = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Extinguisher", "Extinguisher"));
			}

			if (SaveData->CollectArchives.Item9_Cutter)
			{
				CollectedData.Item9_Cutter = true;
				Archive9Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Cutter", "Cutter"));
			}

			if (SaveData->CollectArchives.Item10_SoulLantern)
			{
				CollectedData.Item10_SoulLantern = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_SoulLantern", "Soul Lantern"));
			}

			if (SaveData->CollectArchives.Item11_GlowStick)
			{
				CollectedData.Item11_GlowStick = true;
				Archive11Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_GlowStick", "Glow Stick"));
			}

			Archive11->OnClicked.AddDynamic(this, &UArchiveDocsWidget::OnArchive11ButtonClick);
			Archive11->OnHovered.AddDynamic(this, &UArchiveDocsWidget::OnArchive11ButtonHover);
		}
		else if (ArchiveMode == EArchiveModes::AM_Document)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Archive Mode: AM_Document")));
			NavIndex = 11;

			if (SaveData->CollectArchives.Article1)
			{
				CollectedData.Article1 = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article1", "1/4 Article"));
			}

			if (SaveData->CollectArchives.Article2)
			{
				CollectedData.Article2 = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article2", "12/7 Article"));
			}

			if (SaveData->CollectArchives.Article3)
			{
				CollectedData.Article3 = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article3", "12/18 Article"));
			}

			if (SaveData->CollectArchives.Article4)
			{
				CollectedData.Article4 = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article4", "1/11 Article"));
			}

			if (SaveData->CollectArchives.Article5)
			{
				CollectedData.Article5 = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Article5", "2/6 Article"));
			}

			if (SaveData->CollectArchives.Diary1)
			{
				CollectedData.Diary1 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary1", "11/4 Diary"));
			}

			if (SaveData->CollectArchives.Diary2)
			{
				CollectedData.Diary2 = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary2", "Summon's Diary"));
			}

			if (SaveData->CollectArchives.Diary3)
			{
				CollectedData.Diary3 = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary3", "1/3 Diary"));
			}

			if (SaveData->CollectArchives.Diary4)
			{
				CollectedData.Diary4 = true;
				Archive9Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary4", "12/17 Diary"));
			}

			if (SaveData->CollectArchives.Diary5)
			{
				CollectedData.Diary5 = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Diary5", "2/6 Diary"));
			}

			Archive11->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (ArchiveMode == EArchiveModes::AM_Letter)
		{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Archive Mode: AM_Letter")));
			NavIndex = 11;

			if (SaveData->CollectArchives.Letter1_Berith1)
			{
				CollectedData.Letter1_Berith1 = true;
				Archive1Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter1", "Natty's Letter 1"));
			}

			if (SaveData->CollectArchives.Letter2_Berith2)
			{
				CollectedData.Letter2_Berith2 = true;
				Archive2Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter2", "Natty's Letter 2"));
			}

			if (SaveData->CollectArchives.Letter3_Berith3)
			{
				CollectedData.Letter3_Berith3 = true;
				Archive3Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter3", "Natty's Letter 3"));
			}

			if (SaveData->CollectArchives.Letter4_Berith4)
			{
				CollectedData.Letter4_Berith4 = true;
				Archive4Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter4", "Natty's Letter 4"));
			}

			if (SaveData->CollectArchives.Letter5_Berith5)
			{
				CollectedData.Letter5_Berith5 = true;
				Archive5Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter5", "Natty's Letter 5"));
			}

			if (SaveData->CollectArchives.Letter6_Berith6)
			{
				CollectedData.Letter6_Berith6 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter6", "Natty's Letter 6"));
			}

			if (SaveData->CollectArchives.Letter7_Gamigin1)
			{
				CollectedData.Letter7_Gamigin1 = true;
				Archive7Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter7", "Hongsul's Letter 1"));
			}

			if (SaveData->CollectArchives.Letter8_Gamigin2)
			{
				CollectedData.Letter8_Gamigin2 = true;
				Archive8Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter8", "Hongsul's Letter 2"));
			}

			if (SaveData->CollectArchives.Letter9_Gamigin3)
			{
				CollectedData.Letter9_Gamigin3 = true;
				Archive6Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter9", "Hongsul's Letter 3"));
			}

			if (SaveData->CollectArchives.Letter10_Gamigin4)
			{
				CollectedData.Letter10_Gamigin4 = true;
				Archive10Text->SetText(NSLOCTEXT("UArchiveDocsWidget", "Archive_Letter10", "Hongsul's Letter 4"));
			}

			Archive11->SetVisibility(ESlateVisibility::Collapsed);
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

// Exit 버튼을 눌렀을 때 작동할 함수.
void UArchiveDocsWidget::OnExitButtonClick()
{
	// 버튼 클릭 소리를 출력함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 이 위젯을 뷰포트에 출력하기 이전의 위젯을 가져와 뷰포트에 출력시킴.
	UArchiveWidget* FromWidget = CreateWidget<UArchiveWidget>(GetWorld(), ParentWidget);
	FromWidget->AddToViewport();
	//FromWidget->bIsFocusable = true;
	auto Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FromWidget->SetUserFocus(Controller);
	FromWidget->SetKeyboardFocus();
	
	// 메인 UI에서 왔는지를 체크해 설정하고
	FromWidget->bIsStartGameMode = bIsStartGameMode;
	if (bIsStartGameMode)
	{
		auto GameMode = Cast<AStartGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		GameMode->SetCurrentWidget(FromWidget);
	}
	// 위젯을 뷰포트로부터 제거함.
	RemoveFromParent();
}

// Exit 버튼에 마우스를 올렸을 때 작동할 함수
void UArchiveDocsWidget::OnExitButtonHover()
{
	// 이동 음을 출력하고, 현재 버튼 인덱스를 마지막 인덱스로 설정함
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	CurrentIndex = NavIndex - 1;
	UpdateButtonSlate();
}

// 아이템 패널 나가기 버튼을 눌렀을 때 작동할 함수
void UArchiveDocsWidget::OnItemExitButtonClick()
{
	// 클릭 음을 재생하고 아이템 내용을 보여줄 패널을 뷰포트로부터 제거함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	Exit->SetVisibility(ESlateVisibility::Visible);
	ItemPanel->SetVisibility(ESlateVisibility::Collapsed);
	ArchiveState = EArchiveStates::AS_Search;
}

// 문서 패널 나가기 버튼을 눌렀을 때 작동할 함수.
void UArchiveDocsWidget::OnDocumentExitButtonClick()
{
	// 클릭 음을 재생하고 문서 내용을 보여줄 패널을 뷰포트로부터 제거함.
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

// 1번 문서 버튼을 눌렀을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive1ButtonClick()
{
	// 클릭 음을 재생
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	
	// 현재 아이템 위젯이라면
	if (ArchiveMode == EArchiveModes::AM_Item) 
	{
		// 라이터를 한 번이라도 습득해놓은 상태면 확인할 수 있도록
		if (CollectedData.Item1_CigarLighter) 
		{
			// 아이템 데이터에서 라이터 정보를 가져옴
			FHorrorGameItemData* CigarLightData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(1), TEXT(""));
			FText ItemName = CigarLightData->ItemName;
			UTexture2D* Texture = CigarLightData->ItemIcon;
			FText ItemDetail = CigarLightData->ItemDetail;
			
			// 위젯 상태는 세부 내역 상태로 바꿈
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력시킴.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 1을 한 번이라도 습득해놓은 상태면 확인할 수 있도록
		if (CollectedData.Article1)
		{
			// 아카이브 데이터에서 1번 기사 정보를 가져옴
			FCollectArchivesData* Article1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(1), TEXT(""));

			// 위젯 상태는 세부 내역 상태로 바꿈
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Article1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 1을 한 번이라도 습득해놓은 상태면 확인할 수 있도록
		if (CollectedData.Letter1_Berith1)
		{
			// 아카이브 데이터에서 편지 1의 정보를 가져옴.
			FCollectArchivesData* Letter1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(6), TEXT(""));
			
			// 위젯 상태는 세부 내역 상태로 바꿈.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Letter1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 1번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive1ButtonHover()
{
	// 버튼 이동 음을 재생시킴.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 라이터를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item1_CigarLighter)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_CigarLight", "Description of the lighter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Article1)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article1", "Article dated January 4th obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter1_Berith1)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter1", "First letter from Natty obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 현재 버튼의 인덱스는 0으로 설정하고 버튼 상태를 업데이트함.
	CurrentIndex = 0;

	UpdateButtonSlate();
}

// 2번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive2ButtonClick()
{
	// 클릭음을 재생시킴.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 플래시라이트를 한 번이라도 습득해놓은 상태일 경우에 확인할 수 있도록 설정.
		if (CollectedData.Item2_FlashLight)
		{
			// 아이템 데이터에서 플래시라이트 정보를 가져옴.
			FHorrorGameItemData* FlashLightData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(2), TEXT(""));
			FText ItemName = FlashLightData->ItemName;
			UTexture2D* Texture = FlashLightData->ItemIcon;
			FText ItemDetail = FlashLightData->ItemDetail;

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력시킴.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 2를 한 번이라도 습득해놓은 상태일 경우에 확인할 수 있도록 설정.
		if (CollectedData.Article2)
		{
			// 아카이브 데이터에서 기사 2의 정보를 가져옴.
			FCollectArchivesData* Article2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(2), TEXT(""));

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Article2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 2를 한 번이라도 습득해놓은 상태일 경우 확인할 수 있도록 설정.
		if (CollectedData.Letter2_Berith2)
		{
			// 아카이브 데이터에서 편지 2의 정보를 가져옴
			FCollectArchivesData* Letter2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(7), TEXT(""));

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Letter2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 2번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive2ButtonHover()
{
	// 버튼 이동 음을 재생시킴.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 플래시라이트를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item2_FlashLight)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Flashlight", "Description of the flashlight"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Article2)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article2", "Article dated December 7th obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter2_Berith2)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter2", "Second letter from Natty obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 현재 버튼의 인덱스를 1로 설정하고 버튼 상태를 업데이트 함.
	CurrentIndex = 1;

	UpdateButtonSlate();
}

// 3번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive3ButtonClick()
{
	// 버튼 클릭음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	// 현재 아이템 위젯이라면
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 열쇠를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Item3_Key)
		{
			// 아이템 데이터에서 열쇠 정보를 가져옴.
			FHorrorGameItemData* KeyData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(3), TEXT(""));
			FText ItemName = KeyData->ItemName;
			UTexture2D* Texture = KeyData->ItemIcon;
			FText ItemDetail = KeyData->ItemDetail;

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력시킴.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 3을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Article3)
		{
			// 아카이브 데이터에서 기사 3의 정보를 가져옴.
			FCollectArchivesData* Article3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(3), TEXT(""));

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Article3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 3을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Letter3_Berith3)
		{
			// 아카이브 데이터에서 편지 3의 정보를 가져옴.
			FCollectArchivesData* Letter3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(8), TEXT(""));

			// 위젯 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력시킴.
			Document->SetText(Letter3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 3번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive3ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 열쇠를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item3_Key)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Key", "Description of the key"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Article3)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article3", "Article dated December 18th obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter3_Berith3)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter3", "Third letter from Natty obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 버튼 인덱스를 2로 변경하고 버튼 상태를 업데이트함.
	CurrentIndex = 2;

	UpdateButtonSlate();
}

// 4번 문서를 클릭할 때 작동할 함수.
void UArchiveDocsWidget::OnArchive4ButtonClick()
{
	// 버튼 클릭 음을 재생시킴.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}
	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 타이머를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Item4_Timer)
		{
			// 아이템 데이터에서 타이머의 정보를 가져옴.
			FHorrorGameItemData* TimerData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(4), TEXT(""));
			FText ItemName = TimerData->ItemName;
			UTexture2D* Texture = TimerData->ItemIcon;
			FText ItemDetail = TimerData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 4를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Article4)
		{
			// 아카이브 데이터에서 기사 4의 정보를 가져옴.
			FCollectArchivesData* Article4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(4), TEXT(""));
			
			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Article4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);	
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 4를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Letter4_Berith4)
		{
			// 아카이브 데이터에서 편지 4의 정보를 가져옴.
			FCollectArchivesData* Letter4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(9), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 4번 문서 버튼 위에 마우스를 올렸을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive4ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 타이머를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item4_Timer)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Timer", "Description of the timer"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Article4)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article4", "Article dated January 11th obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter4_Berith4)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter4", "Fourth letter from Natty obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 버튼 인덱스를 3으로 변경하고 버튼 상태를 업데이트 함.
	CurrentIndex = 3;

	UpdateButtonSlate();
}

// 5번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive5ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 검을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Item5_BronzeSword)
		{
			// 아이템 데이터에서 청동 검의 정보를 가져옴.
			FHorrorGameItemData* BronzeSwordData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(5), TEXT(""));
			FText ItemName = BronzeSwordData->ItemName;
			UTexture2D* Texture = BronzeSwordData->ItemIcon;
			FText ItemDetail = BronzeSwordData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 5를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정,
		if (CollectedData.Article5)
		{
			// 아카이브 데이터에서 기사 5의 정보를 가져옴
			FCollectArchivesData* Article5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(5), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Article5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::White);
			Article->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 5를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정.
		if (CollectedData.Letter5_Berith5)
		{
			// 아카이브 데이터에서 편지 5의 정보를 가져옴.
			FCollectArchivesData* Letter5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(10), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 5번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive5ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 검을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item5_BronzeSword)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Sword", "Description of the bronze sword"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 기사 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Article5)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article5", "Article dated February 6th obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter5_Berith5)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter5", "Fifth letter from Natty obtained from the 'Hallway of Shadows' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 버튼 인덱스를 4로 변경하고 버튼 상태를 업데이트함.
	CurrentIndex = 4;

	UpdateButtonSlate();
}

// 6번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive6ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 방울을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item6_BronzeBell)
		{
			// 아이템 데이터에서 청동 방울의 정보를 가져옴.
			FHorrorGameItemData* BronzeBellData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(6), TEXT(""));
			FText ItemName = BronzeBellData->ItemName;
			UTexture2D* Texture = BronzeBellData->ItemIcon;
			FText ItemDetail = BronzeBellData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 1을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Diary1)
		{
			// 아카이브 데이터에서 일기 1의 정보를 가져옴.
			FCollectArchivesData* Diary1Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(16), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Diary1Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 6을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Letter6_Berith6)
		{
			// 아카이브 데이터에서 편지 6의 정보를 가져옴.
			FCollectArchivesData* Letter6Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(11), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter6Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 6번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive6ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 방울을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item6_BronzeBell)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Bell", "Description of the bronze bell"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Diary1)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary1", "Diary dated November 4th obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 6을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter6_Berith6)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter6", "Sixth letter from Natty obtained from the 'Hallway of Shadows' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 버튼 인덱스를 5로 변경하고 버튼의 상태를 업데이트 함.
	CurrentIndex = 5;

	UpdateButtonSlate();
}

// 7번 문서 버튼을 클릭한 경우 작동할 함수.
void UArchiveDocsWidget::OnArchive7ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 거울을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item7_BronzeMirror)
		{
			// 아이템 데이터에서 청동 거울의 정보를 가져옴.
			FHorrorGameItemData* BronzeMirrorData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(7), TEXT(""));
			FText ItemName = BronzeMirrorData->ItemName;
			UTexture2D* Texture = BronzeMirrorData->ItemIcon;
			FText ItemDetail = BronzeMirrorData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 2를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Diary2)
		{
			// 아카이브 데이터에서 일기 2의 정보를 가져옴.
			FCollectArchivesData* Diary2Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(17), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Diary2Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 7을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Letter7_Gamigin1)
		{
			// 아카이브 데이터에서 편지 7의 정보를 가져옴.
			FCollectArchivesData* Letter7Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(12), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter7Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 7번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive7ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 청동 거울을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item7_BronzeMirror)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Mirror", "Description of the bronze mirror"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Diary2)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary2", "Someone's diary obtained from the 'Basement' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 7을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter7_Gamigin1)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter7", "First letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 버튼의 인덱스를 6으로 변경하고 버튼의 상태를 업데이트 함.
	CurrentIndex = 6;

	UpdateButtonSlate();
}

// 8번 문서 버튼을 클릭한 경우 작동할 함수.
void UArchiveDocsWidget::OnArchive8ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 소화기를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item8_Extinguisher)
		{
			// 아이템 데이터에서 소화기의 정보를 가져옴.
			FHorrorGameItemData* ExtinguisherData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(8), TEXT(""));
			FText ItemName = ExtinguisherData->ItemName;
			UTexture2D* Texture = ExtinguisherData->ItemIcon;
			FText ItemDetail = ExtinguisherData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 3을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Diary3)
		{
			// 아카이브 데이터에서 일기 3의 정보를 가져옴.
			FCollectArchivesData* Diary3Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(18), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Diary3Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 8을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Letter8_Gamigin2)
		{
			// 아카이브 데이터에서 편지 8의 정보를 가져옴.
			FCollectArchivesData* Letter8Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(13), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter8Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 8번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive8ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}
	
	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 소화기를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item8_Extinguisher)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Extinguisher", "Description of the extinguisher"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Diary3)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary3", "Diary dated January 3rd obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 8을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter8_Gamigin2)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter8", "Second letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	
	// 버튼 인덱스를 7로 변경하고 버튼의 상태를 업데이트함.
	CurrentIndex = 7;

	UpdateButtonSlate();
}

// 9번 문서 버튼을 클릭한 경우 작동할 함수.
void UArchiveDocsWidget::OnArchive9ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 절단기를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item9_Cutter)
		{
			// 아이템 데이터에서 절단기의 정보를 가져옴.
			FHorrorGameItemData* CutterData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(9), TEXT(""));
			FText ItemName = CutterData->ItemName;
			UTexture2D* Texture = CutterData->ItemIcon;
			FText ItemDetail = CutterData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 4를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Diary4)
		{
			// 아카이브 데이터에서 일기 4의 정보를 가져옴.
			FCollectArchivesData* Diary4Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(19), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Diary4Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 9를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Letter9_Gamigin3)
		{
			// 아카이브 데이터에서 편지 9의 정보를 가져옴.
			FCollectArchivesData* Letter9Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(14), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter9Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 9번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive9ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 절단기를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item9_Cutter)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Cutter", "Description of the cutter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Diary4)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary4", "Diary dated December 17th obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 9를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter9_Gamigin3)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter9", "Third letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}

	// 현재 버튼 인덱스를 8로 변경하고 버튼의 상태를 업데이트 함.
	CurrentIndex = 8;

	UpdateButtonSlate();
}

// 10번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive10ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 영혼 랜턴을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item10_SoulLantern)
		{
			// 아이템 데이터에서 영혼 랜턴의 정보를 가져옴.
			FHorrorGameItemData* SoulLanternData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(10), TEXT(""));
			FText ItemName = SoulLanternData->ItemName;
			UTexture2D* Texture = SoulLanternData->ItemIcon;
			FText ItemDetail = SoulLanternData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 5를 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Diary5)
		{
			// 아카이브 데이터에서 일기 5의 정보를 가져옴.
			FCollectArchivesData* Diary5Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(20), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Diary5Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Document->SetColorAndOpacity(FColor::Black);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 10을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Letter10_Gamigin4)
		{
			// 아카이브 데이터에서 편지 10의 정보를 가져옴.
			FCollectArchivesData* Letter10Data = ArchiveData->FindRow<FCollectArchivesData>(*FString::FromInt(15), TEXT(""));

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_DocumentDetail;

			// 문서 패널의 텍스트에 가져온 정보를 입력함.
			Document->SetText(Letter10Data->Detail_EN);
			DocumentPanel->SetVisibility(ESlateVisibility::Visible);
			Letter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

// 10번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive10ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 영혼 랜턴을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item10_SoulLantern)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Lantern", "Description of the soul lantern"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 일기 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Diary5)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary5", "Diary dated February 6th obtained from the 'Hallway of Shadows' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지 10을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Letter10_Gamigin4)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter10", "Fourth letter from Hongsul obtained from the 'Mist Estate' Chapter"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	
	// 현재 버튼 인덱스를 9로 변경하고 버튼의 상태를 업데이트 함.
	CurrentIndex = 9;

	UpdateButtonSlate();
}

// 11번 문서 버튼을 클릭했을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive11ButtonClick()
{
	// 버튼 클릭 음을 재생함.
	if (IsValid(ButtonClickSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonClickSound);
	}

	// 현재 아이템 위젯이라면
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 야광봉을 한 번이라도 습득해놓은 상태라면 확인할 수 있도록 설정함.
		if (CollectedData.Item11_GlowStick)
		{
			// 아이템 데이터에서 야광봉의 정보를 가져옴.
			FHorrorGameItemData* GlowStickData = ItemData->FindRow<FHorrorGameItemData>(*FString::FromInt(11), TEXT(""));
			FText ItemName = GlowStickData->ItemName;
			UTexture2D* Texture = GlowStickData->ItemIcon;
			FText ItemDetail = GlowStickData->ItemDetail;

			// 위젯의 상태를 세부 내역 상태로 변경함.
			ArchiveState = EArchiveStates::AS_Detail;

			// 아이템 패널의 이미지와 텍스트에 가져온 정보를 입력함.
			ItemImage->SetBrushFromTexture(Texture, true);
			Title->SetText(ItemName);
			Detail->SetText(ItemDetail);
			ItemPanel->SetVisibility(ESlateVisibility::Visible);
			Exit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 현재 문서 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 문서가 추가되면 추가할 것
	}
	// 현재 편지 위젯이라면
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지가 추가되면 추가할 것
	}
}

// 11번 문서 버튼 위에 마우스를 올려두었을 때 작동할 함수.
void UArchiveDocsWidget::OnArchive11ButtonHover()
{
	// 버튼 이동 음을 재생함.
	if (IsValid(ButtonMoveSound))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
	}

	// 현재 아이템 위젯인 경우
	if (ArchiveMode == EArchiveModes::AM_Item)
	{
		// 야광봉을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
		if (CollectedData.Item11_GlowStick)
		{
			ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_GlowStick", "Description of the glow stick"));
		}
		else
		{
			ExplainText->SetText(NSLOCTEXT("UHintWidget", "Hover_None", ""));

		}
	}
	// 현재 문서 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Document)
	{
		// 문서가 추가되면 추가할 것
	}
	// 현재 편지 위젯인 경우
	else if (ArchiveMode == EArchiveModes::AM_Letter)
	{
		// 편지가 추가되면 추가할 것
	}
	
	// 현재 버튼 인덱스를 10으로 변경하고 버튼의 상태도 업데이트 함
	CurrentIndex = 10;

	UpdateButtonSlate();
}

void UArchiveDocsWidget::AnimationFinished()
{
	RemoveFromParent();
}

// 버튼의 상태를 업데이트하는 함수.
void UArchiveDocsWidget::UpdateButtonSlate()
{
	switch (CurrentIndex)
	{
		// 현재 버튼 인덱스가 0일 때, 1번 문서의 버튼만 흰색으로 설정함.
		case 0:
		{
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
		
			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 라이터를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item1_CigarLighter)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_CigarLight", "Description of the lighter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 기사 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Article1)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article1", "Article dated January 4th obtained from the 'Basement' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter1_Berith1)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter1", "First letter from Natty obtained from the 'Basement' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 1일 때, 2번 문서의 버튼만 흰색으로 설정함.
		case 1:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 플래시라이트를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item2_FlashLight)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Flashlight", "Description of the flashlight"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 기사 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Article2)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article2", "Article dated December 7th obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter2_Berith2)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter2", "Second letter from Natty obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 2일 때, 3번 문서의 버튼만 흰색으로 설정함.
		case 2:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 열쇠를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item3_Key)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Key", "Description of the key"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 기사 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Article3)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article3", "Article dated December 18th obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter3_Berith3)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter3", "Third letter from Natty obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 3일 때, 4번 문서의 버튼만 흰색으로 설정함.
		case 3:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 타이머를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item4_Timer)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Timer", "Description of the timer"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 기사 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠
				if (CollectedData.Article4)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article4", "Article dated January 11th obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter4_Berith4)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter4", "Fourth letter from Natty obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 4일 때, 5번 문서의 버튼만 흰색으로 설정함.
		case 4:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 청동 검을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item5_BronzeSword)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Sword", "Description of the bronze sword"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 기사 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Article5)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Article5", "Article dated February 6th obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter5_Berith5)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter5", "Fifth letter from Natty obtained from the 'Hallway of Shadows' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 5일 때, 6번 문서의 버튼만 흰색으로 설정함.
		case 5:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 청동 방울을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item6_BronzeBell)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Bell", "Description of the bronze bell"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 일기 1을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Diary1)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary1", "Diary dated November 4th obtained from the 'Basement' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 6을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter6_Berith6)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter6", "Sixth letter from Natty obtained from the 'Hallway of Shadows' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 6일 때, 7번 문서의 버튼만 흰색으로 설정함.
		case 6:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 청동 거울을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item7_BronzeMirror)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Mirror", "Description of the bronze mirror"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 일기 2를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Diary2)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary2", "Someone's diary obtained from the 'Basement' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 7을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter7_Gamigin1)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter7", "First letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 7일 때, 8번 문서의 버튼만 흰색으로 설정함.
		case 7:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 소화기를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item8_Extinguisher)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Extinguisher", "Description of the extinguisher"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 일기 3을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Diary3)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary3", "Diary dated January 3rd obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 8을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter8_Gamigin2)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter8", "Second letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 8일 때, 9번 문서의 버튼만 흰색으로 설정함.
		case 8:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 절단기를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item9_Cutter)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Cutter", "Description of the cutter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 일기 4를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Diary4)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary4", "Diary dated December 17th obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯이라면
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 9를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter9_Gamigin3)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter9", "Third letter from Hongsul obtained from the 'School of Labyrinth' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 9일 때, 10번 문서의 버튼만 흰색으로 설정함.
		case 9:
		{
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

			// 현재 아이템 위젯인 경우
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				// 영혼 랜턴을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item10_SoulLantern)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Lantern", "Description of the soul lantern"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 문서 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Document)
			{
				// 일기 5를 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Diary5)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Diary5", "Diary dated February 6th obtained from the 'Hallway of Shadows' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 현재 편지 위젯인 경우
			else if (ArchiveMode == EArchiveModes::AM_Letter)
			{
				// 편지 10을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Letter10_Gamigin4)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Letter10", "Fourth letter from Hongsul obtained from the 'Mist Estate' Chapter"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			return;
		}
		// 현재 버튼 인덱스가 10일 때, 아이템 위젯이면 11번 문서의 버튼만, 아니라면 뒤로가기 버튼만 흰색으로 설정함.
		case 10:
		{
			// 현재 아이템 위젯이라면
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
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
				Archive11->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
				Exit->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));

				// 야광봉을 한 번이라도 얻은 상태라면 기본 설명란에 문구를 추가하고 아니면 비워둠.
				if (CollectedData.Item11_GlowStick)
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_GlowStick", "Description of the glow stick"));
				}
				else
				{
					ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_None", ""));
				}
			}
			// 아이템 위젯이 아닌 다른 위젯이라면
			else
			{
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
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Exit", ""));
			}
			return;
		}
		// 현재 버튼 인덱스가 11이라면 아이템 위젯에서만 뒤로가기 버튼을 흰색으로 설정함. 
		case 11:
		{
			if (ArchiveMode == EArchiveModes::AM_Item)
			{
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
				Archive11->SetColorAndOpacity(FLinearColor(0.4f, 0.4f, 0.4f, 1.f));
				Exit->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

				ExplainText->SetText(NSLOCTEXT("UArchiveDocsWidget", "Hover_Exit", ""));
			}
			return;
		}
	}
}

// 유저가 키 입력시 이를 감지할 함수.
FReply UArchiveDocsWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력한 키의 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();

	// 입력한 키가 엔터인 경우
	if (KeyType == "Enter")
	{
		// 현재 위젯의 상태가 선택하기 상태라면
		if (ArchiveState == EArchiveStates::AS_Search) 
		{
			switch (CurrentIndex)
			{
				// 현재 버튼 인덱스가 0이면 1번 문서 클릭 시 실행될 함수를 실행함.
				case 0:
				{
					OnArchive1ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 1이면 2번 문서 클릭 시 실행될 함수를 실행함.
				case 1:
				{
					OnArchive2ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 2이면 3번 문서 클릭 시 실행될 함수를 실행함.
				case 2:
				{
					OnArchive3ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 3이면 4번 문서 클릭 시 실행될 함수를 실행함.
				case 3:
				{
					OnArchive4ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 4이면 5번 문서 클릭 시 실행될 함수를 실행함.
				case 4:
				{
					OnArchive5ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 5이면 6번 문서 클릭 시 실행될 함수를 실행함.
				case 5:
				{
					OnArchive6ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 6이면 7번 문서 클릭 시 실행될 함수를 실행함.
				case 6:
				{
					OnArchive7ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 7이면 8번 문서 클릭 시 실행될 함수를 실행함.
				case 7:
				{
					OnArchive8ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 8이면 9번 문서 클릭 시 실행될 함수를 실행함.
				case 8:
				{
					OnArchive9ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 9이면 10번 문서 클릭 시 실행될 함수를 실행함.
				case 9:
				{
					OnArchive10ButtonClick();
					break;
				}
				// 현재 버튼 인덱스가 10이면 아이템 위젯인 경우 11번 문서 클릭 시 실행될 함수가, 아니라면 뒤로가기 버튼 클릭 시 실행될 함수를 실행함.
				case 10:
				{
					if (ArchiveMode == EArchiveModes::AM_Item)
					{
						OnArchive11ButtonClick();
					}
					else
					{
						OnExitButtonClick();
					}
					break;
				}
				// 현재 버튼 인덱스가 11이면 뒤로 가기 버튼을 클릭 시 실행될 함수를 실행함.
				case 11:
				{
					OnExitButtonClick();
					break;
				}
			}
		}
		// 현재 위젯의 상태가 세부 내용 확인 상태라면 아이템 패널 나가기 버튼 클릭할 때 실행될 함수를 실행함.
		else if(ArchiveState == EArchiveStates::AS_Detail)
		{
			OnItemExitButtonClick();
		}
		// 현재 위젯의 상태가 세부 내용 확인 상태라면 문서 패널 나가기 버튼 클릭할 때 실행될 함수를 실행함.
		else if (ArchiveState == EArchiveStates::AS_DocumentDetail)
		{
			OnDocumentExitButtonClick();
		}
	}
	// 엔터가 아닌 경우
	else
	{
		// D 키나 우측 화살표 키라면
		if (KeyType == "D" || KeyType == "Right")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}
			
			// 현재 버튼 인덱스를 1 증가시키고, 최대 인덱스를 넘어갈 경우, 0으로 변경함.
			CurrentIndex++;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex = 0;
			}
		}
		// A 키나 좌측 화살표 키라면
		else if (KeyType == "A" || KeyType == "Left")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 1 감소시키고, 인덱스가 0 미만일 경우, 최대 인덱스로 변경함.
			CurrentIndex--;
			if (CurrentIndex < 0)
			{
				CurrentIndex = NavIndex - 1;
			}
		}
		// S 키나 아래 화살표 키라면
		if (KeyType == "S" || KeyType == "Down")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 2 증가시키고, 인덱스가 최대 인덱스를 넘어가면, 최대 인덱스만큼 뺌.
			CurrentIndex += 2;
			if (CurrentIndex >= NavIndex)
			{
				CurrentIndex -= NavIndex;
			}
		}
		// W 키나 위 화살표 키라면
		else if (KeyType == "W" || KeyType == "Up")
		{
			// 버튼 이동 음을 재생함.
			if (IsValid(ButtonMoveSound))
			{
				UGameplayStatics::PlaySound2D(GetWorld(), ButtonMoveSound);
			}

			// 현재 버튼 인덱스를 2 감소시키고, 인덱스가 0 미만일 경우, 최대 인덱스만큼 더함.
			CurrentIndex -= 2;
			if (CurrentIndex < 0)
			{
				CurrentIndex += NavIndex;
			}
		}

		// 그 후 버튼의 상태를 업데이트 함.
		UpdateButtonSlate();
	}

	return reply;
}