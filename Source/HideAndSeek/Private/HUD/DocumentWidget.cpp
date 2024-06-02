// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "HUD/DocumentWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "HorrorGamePlayercontroller.h"
#include "Kismet/GameplayStatics.h"

// 생성자.
void UDocumentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼 별로 클릭 시 실행할 함수를 바인딩함.
	Exit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickExitButton);
	ArticleExit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickArticleExitButton);
	TipsExit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickTipsExitButton);

	Letter_Diary->SetVisibility(ESlateVisibility::Collapsed);
	ArticleBorder->SetVisibility(ESlateVisibility::Collapsed);
	TipsBorder->SetVisibility(ESlateVisibility::Collapsed);
}

// 편지나 일기와 같은 문서를 위한 텍스트 설정 함수.
void UDocumentWidget::SetDocumentText(FText text)
{
	Document->SetText(text);
}

// 문서 끄기 버튼을 클릭했을 때 작동할 함수.
void UDocumentWidget::OnClickExitButton()
{
	// 플레이어 컨트롤러를 가져와서 인풋이 되도록 설정하고, 일시 정지도 해제함.
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	// 그 후 뷰포트에서 이 위젯을 제거함.
	RemoveFromParent();
}

// 기사와 같은 문서를 위한 텍스트 설정 함수.
void UDocumentWidget::SetArticleText(FText text)
{
	ArticleText->SetText(text);
}

// 기사 끄기 버튼을 클릭했을 때 작동할 함수.
void UDocumentWidget::OnClickArticleExitButton()
{
	// 플레이어 컨트롤러를 가져와서 인풋이 되도록 설정하고, 일시 정지도 해제함.
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	// 그 후 뷰포트에서 이 위젯을 제거함.
	RemoveFromParent();
}

// 힌트와 같은 문서를 위한 텍스트 설정 함수.
void UDocumentWidget::SetTipsText(FText text)
{
	TipsText->SetText(text);
}

// 힌트 끄기 버튼을 클릭했을 때 작동할 함수.
void UDocumentWidget::OnClickTipsExitButton()
{
	// 플레이어 컨트롤러를 가져와서 인풋이 되도록 설정하고, 일시 정지도 해제함.
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	// 그 후 뷰포트에서 이 위젯을 제거함.
	RemoveFromParent();
}

// 위젯의 상태를 설정하는 함수.
void UDocumentWidget::SetCurrentDocumentMode(EDocument inDocumentMode)
{
	DocumentMode = inDocumentMode;

	switch (DocumentMode)
	{
		// 현재 위젯 상태가 힌트를 출력하는 상태면, 힌트 패널만 보이게 설정함.
		case EDocument::D_Hint:
		{
			TipsBorder->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		// 현재 위젯 상태가 기사를 출력하는 상태면, 기사 패널만 보이게 설정함.
		case EDocument::D_Article:
		{
			ArticleBorder->SetVisibility(ESlateVisibility::Visible);
			return;
		}
		// 현재 위젯 상태가 편지나 일기를 출력하는 상태면, 편지나 일기 패널만 보이게 설정함.
		case EDocument::D_Letter:
		{
			Letter_Diary->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}
}

// 키보드 입력을 받았을 때 수행할 함수.
FReply UDocumentWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	// 입력받은 키 코드를 가져옴.
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();
	
	// 엔터 키인 경우
	if (KeyType == "Enter")
	{
		switch (DocumentMode)
		{
			// 현재 힌트 위젯인 경우, 힌트 끄기 버튼을 누른 효과를 줌.
			case EDocument::D_Hint:
			{
				OnClickTipsExitButton();
				break;
			}
			// 현재 기사 위젯인 경우, 기사 끄기 버튼을 누른 효과를 줌.
			case EDocument::D_Article:
			{
				OnClickArticleExitButton();
				break;
			}
			// 현재 편지/일기 위젯인 경우, 편지/일기 끄기 버튼을 누른 효과를 줌.
			case EDocument::D_Letter:
			{
				OnClickExitButton();
				break;
			}
		}
	}

	return reply;
}