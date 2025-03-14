// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Resource/CustomDataTables.h"
#include "ComponentAction/CollectArchives.h"
#include "ArchiveDocsWidget.generated.h"

UENUM(BlueprintType)
enum class EArchiveModes : uint8
{
	AM_Hint UMETA(DisplayName = "Hint"),
	AM_Item UMETA(DisplayName = "Item"),
	AM_Document UMETA(DisplayName = "Document"),
	AM_Letter UMETA(DisplayName = "Explain"),
}; // 현재 기록 보관함의 상태(힌트 / 아이템 / 기사+일기 / 편지)

UENUM(BlueprintType)
enum class EArchiveStates : uint8
{
	AS_Search UMETA(DisplayName = "Search"),
	AS_Detail UMETA(DisplayName = "Detail"),
	AS_DocumentDetail UMETA(DisplayName = "DocumentDetail"),
}; // 현재 기록 보관함의 세부 상태(문서 항목 확인 / 아이템 세부 내용 확인 / 기타 문서 세부 내용 확인)
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UArchiveDocsWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
		void AnimationFinished();

	UFUNCTION(BlueprintCallable)
		void OnExitButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnExitButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnItemExitButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnDocumentExitButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive1ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive1ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive2ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive2ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive3ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive3ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive4ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive4ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive5ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive5ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive6ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive6ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive7ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive7ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive8ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive8ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive9ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive9ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive10ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive10ButtonHover();

	UFUNCTION(BlueprintCallable)
		void OnArchive11ButtonClick();

	UFUNCTION(BlueprintCallable)
		void OnArchive11ButtonHover();

	UFUNCTION(BlueprintCallable)
		void UpdateButtonSlate();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* CreateAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* RemoveAnim;

	// 아이템 아카이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UBorder* ItemPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* ItemExit;

	// 문서(기사, 일기, 편지) 아카이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UBorder* DocumentPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Document;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* DocumentExit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UImage> Article;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		TObjectPtr<class UImage> Letter;

	// 목록 아카이브
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Exit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive1Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive2Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive3Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive4Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive5Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive6Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive7Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive8Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive9Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Archive10Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Archive11;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive11Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ExplainText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentIndex; // 현재 버튼 인덱스

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NavIndex; // 전체 인덱스

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 LanguageIndex; // 현재 언어 설정

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EArchiveModes ArchiveMode; // 현재 위젯 설정 <= 이거에 맞게 위젯 내용 변경 예정

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EArchiveStates ArchiveState; // 현재 위젯 설정 <= 이거에 맞게 위젯 내용 변경 예정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ItemData; // 아이템 정보

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* ArchiveData; // 문서 정보

	// 아이템 관련 문서
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FCollectedArchives CollectedData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	bool bIsStartGameMode = true;
};
