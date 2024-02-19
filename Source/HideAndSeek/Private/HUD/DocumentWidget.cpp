// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "HUD/DocumentWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "HorrorGamePlayercontroller.h"
#include "Kismet/GameplayStatics.h"

void UDocumentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Exit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickExitButton);
	ArticleExit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickArticleExitButton);
	TipsExit->OnClicked.AddDynamic(this, &UDocumentWidget::OnClickTipsExitButton);

	Letter_Diary->SetVisibility(ESlateVisibility::Collapsed);
	ArticleBorder->SetVisibility(ESlateVisibility::Collapsed);
	TipsBorder->SetVisibility(ESlateVisibility::Collapsed);
}

void UDocumentWidget::SetDocumentText(FText text)
{
	Document->SetText(text);
}

void UDocumentWidget::OnClickExitButton()
{
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	RemoveFromParent();
}

void UDocumentWidget::SetArticleText(FText text)
{
	ArticleText->SetText(text);
}

void UDocumentWidget::OnClickArticleExitButton()
{
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	RemoveFromParent();
}

void UDocumentWidget::SetTipsText(FText text)
{
	TipsText->SetText(text);
}

void UDocumentWidget::OnClickTipsExitButton()
{
	auto PlayerController = Cast<AHorrorGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)); // 플레이어 컨트롤러 가져오기

	PlayerController->ChangeInputMode(true);
	PlayerController->SetPause(false);

	RemoveFromParent();
}

void UDocumentWidget::SetCurrentDocumentMode(EDocument inDocumentMode)
{
	DocumentMode = inDocumentMode;

	switch (DocumentMode)
	{
	case EDocument::D_Hint:
		TipsBorder->SetVisibility(ESlateVisibility::Visible);
		break;
	case EDocument::D_Article:
		ArticleBorder->SetVisibility(ESlateVisibility::Visible);
		break;
	case EDocument::D_Letter:
		Letter_Diary->SetVisibility(ESlateVisibility::Visible);
		break;
	}
}

FReply UDocumentWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	FKey KeyCode = InKeyEvent.GetKey();
	FString KeyType = KeyCode.ToString();
	
	if (KeyType == "Enter")
	{
		switch (DocumentMode)
		{
		case EDocument::D_Hint:
			OnClickTipsExitButton();
			break;
		case EDocument::D_Article:
			OnClickArticleExitButton();
			break;
		case EDocument::D_Letter:
			OnClickExitButton();
			break;
		}
	}

	return reply;
}