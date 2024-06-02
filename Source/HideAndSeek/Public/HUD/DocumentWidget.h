// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DocumentWidget.generated.h"

UENUM(BlueprintType)
enum class EDocument : uint8
{
	D_Hint UMETA(DisplayName = "Hint"),
	D_Article UMETA(DisplayName = "Article"),
	D_Letter UMETA(DisplayName = "Letter"),
}; // 현재 기록 보관함의 상태(힌트 / 기사 / 편지+일기)
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UDocumentWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	// 편지나 일기를 보는 상태에서의 문서 내용
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UBorder> Letter_Diary;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UTextBlock> Document;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UButton> Exit;

	// 기사를 보는 상태에서의 문서 내용
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UBorder> ArticleBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UTextBlock> ArticleText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UButton> ArticleExit;

	// 힌트를 보는 상태에서의 문서 내용
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UBorder> TipsBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UTextBlock> TipsText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UButton> TipsExit;

public:
	UPROPERTY()
		EDocument DocumentMode; // 어떤 일지를 읽고 있는지 체크

public: 
	UFUNCTION(BlueprintCallable)
		void SetDocumentText(FText text);

	UFUNCTION(BlueprintCallable)
		void OnClickExitButton();

	UFUNCTION(BlueprintCallable)
		void SetArticleText(FText text);

	UFUNCTION(BlueprintCallable)
		void OnClickArticleExitButton();

	UFUNCTION(BlueprintCallable)
		void SetTipsText(FText text);

	UFUNCTION(BlueprintCallable)
		void OnClickTipsExitButton();

	UFUNCTION(BlueprintCallable)
		void SetCurrentDocumentMode(EDocument inDocumentMode);
};
