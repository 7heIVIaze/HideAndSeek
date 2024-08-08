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
	UFUNCTION()
		void AnimationFinished();

	UFUNCTION()
		void OnExitButtonClick();

	UFUNCTION()
		void OnExitButtonHover();

	UFUNCTION()
		void OnItemExitButtonClick();

	UFUNCTION()
		void OnDocumentExitButtonClick();

	UFUNCTION()
		void OnArchive1ButtonClick();

	UFUNCTION()
		void OnArchive1ButtonHover();

	UFUNCTION()
		void OnArchive2ButtonClick();

	UFUNCTION()
		void OnArchive2ButtonHover();

	UFUNCTION()
		void OnArchive3ButtonClick();

	UFUNCTION()
		void OnArchive3ButtonHover();

	UFUNCTION()
		void OnArchive4ButtonClick();

	UFUNCTION()
		void OnArchive4ButtonHover();

	UFUNCTION()
		void OnArchive5ButtonClick();

	UFUNCTION()
		void OnArchive5ButtonHover();

	UFUNCTION()
		void OnArchive6ButtonClick();

	UFUNCTION()
		void OnArchive6ButtonHover();

	UFUNCTION()
		void OnArchive7ButtonClick();

	UFUNCTION()
		void OnArchive7ButtonHover();

	UFUNCTION()
		void OnArchive8ButtonClick();

	UFUNCTION()
		void OnArchive8ButtonHover();

	UFUNCTION()
		void OnArchive9ButtonClick();

	UFUNCTION()
		void OnArchive9ButtonHover();

	UFUNCTION()
		void OnArchive10ButtonClick();

	UFUNCTION()
		void OnArchive10ButtonHover();

	UFUNCTION()
		void OnArchive11ButtonClick();

	UFUNCTION()
		void OnArchive11ButtonHover();

	UFUNCTION()
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

	UPROPERTY()
		int32 CurrentIndex; // 현재 버튼 인덱스

	UPROPERTY()
		int32 NavIndex; // 전체 인덱스

	UPROPERTY()
		FWidgetAnimationDynamicEvent EndDelegate;

	UPROPERTY()
		int32 LanguageIndex; // 현재 언어 설정

	UPROPERTY()
		EArchiveModes ArchiveMode; // 현재 위젯 설정 <= 이거에 맞게 위젯 내용 변경 예정

	UPROPERTY()
		EArchiveStates ArchiveState; // 현재 위젯 설정 <= 이거에 맞게 위젯 내용 변경 예정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UDataTable* ItemData; // 아이템 정보

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		UDataTable* ArchiveData; // 문서 정보

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<UUserWidget> DocumentWidget;*/

	// 아이템 관련 문서
	UPROPERTY()
	FCollectedArchives CollectedData;

	//UPROPERTY()
	//	bool Item1_CigarLighter; // 라이터

	//UPROPERTY()
	//	bool Item2_FlashLight; // 플래시라이터

	//UPROPERTY()
	//	bool Item3_Key; // 열쇠

	//UPROPERTY()
	//	bool Item4_Timer; // 타이머

	//UPROPERTY()
	//	bool Item5_BronzeSword; // 청동검

	//UPROPERTY()
	//	bool Item6_BronzeBell; // 청동 방울

	//UPROPERTY()
	//	bool Item7_BronzeMirror; // 청동 거울

	//UPROPERTY()
	//	bool Item8_Extinguisher; // 소화기

	//UPROPERTY()
	//	bool Item9_Cutter; // 절단기

	//UPROPERTY()
	//	bool Item10_SoulLantern; // 영혼 랜턴

	//UPROPERTY()
	//	bool Item11_GlowStick; // 야광봉

	//// 신문 기사 문서
	//UPROPERTY()
	//	bool Article1; // 1번 기사

	//UPROPERTY()
	//	bool Article2; // 2번 기사

	//UPROPERTY()
	//	bool Article3; // 3번 기사

	//UPROPERTY()
	//	bool Article4; // 4번 기사

	//UPROPERTY()
	//	bool Article5; // 5번 기사

	//// 편지 문서
	//UPROPERTY()
	//	bool Letter1_Berith1; // 베리스의 편지 1

	//UPROPERTY()
	//	bool Letter2_Berith2; // 베리스의 편지 2

	//UPROPERTY()
	//	bool Letter3_Berith3; // 베리스의 편지 3

	//UPROPERTY()
	//	bool Letter4_Berith4; // 베리스의 편지 4

	//UPROPERTY()
	//	bool Letter5_Berith5; // 베리스의 편지 5

	//UPROPERTY()
	//	bool Letter6_Berith6; // 베리스의 편지 6

	//UPROPERTY()
	//	bool Letter7_Gamigin1; // 가미긴의 편지 1 

	//UPROPERTY()
	//	bool Letter8_Gamigin2; // 가미긴의 편지 2 

	//UPROPERTY()
	//	bool Letter9_Gamigin3; // 가미긴의 편지 3 

	//UPROPERTY()
	//	bool Letter10_Gamigin4; // 가미긴의 편지 4

	//// 일기 문서
	//UPROPERTY()
	//	bool Diary1; // 일기 1

	//UPROPERTY()
	//	bool Diary2; // 일기 2

	//UPROPERTY()
	//	bool Diary3; // 일기 3

	//UPROPERTY()
	//	bool Diary4; // 일기 4

	//UPROPERTY()
	//	bool Diary5; // 일기 5

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	bool bIsStartGameMode = true;
};
