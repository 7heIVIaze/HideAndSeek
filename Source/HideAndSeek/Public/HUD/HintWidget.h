// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComponentAction/CreatureType.h"
#include "HintWidget.generated.h"

UENUM(BlueprintType)
enum class EHintStates : uint8
{
	AS_Search UMETA(DisplayName = "Search"),
	AS_Detail UMETA(DisplayName = "Detail"),
}; // 현재 도감의 세부 상태(문서 항목 확인 / 세부 내용 확인)
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UHintWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

public:
	UFUNCTION(BlueprintCallable)
	void OnExitButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnExitButtonHover();

	UFUNCTION(BlueprintCallable)
	void OnCreatureExitButtonClick();

	UFUNCTION(BlueprintCallable)
	void OnHintExitButtonClick();

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
	void UpdateButtonSlate();

public:
	// 크리쳐 견문록
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* CreaturePanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* CreatureImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* CreatureName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Detail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CreatureExit;

	// 도움말
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBorder* HintPanel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HintText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* HintExit;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Exit;

	// 견문록 타이틀
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive1Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive2;

	UPROPERTY( BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive2Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive3;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive3Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive4;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive4Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive5Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Archive6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Archive6Text;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ExplainText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentIndex; // 현재 버튼 인덱스

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 NavIndex; // 전체 인덱스

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EHintStates HintState; // 현재 위젯 설정 <= 이거에 맞게 위젯 내용 변경 예정

	// 크리쳐에게 잡혔는지 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CatchedByReaper; // 리퍼에게 죽었는가

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CatchedByRunner; // 러너에게 죽었는가

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CatchedByBrute; // 브루트에게 죽었는가

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CatchedByShadow; // 그슨대에게 죽었는가

	// 기타 도움말 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool SeeCharacteristic; // 크리쳐의 특징을 보았는가

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool SeeHowToEscape; // 피하는 방법을 보았는가

	// 도감 문서 정보
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UDataTable* HintData; // 크리처 및 도움말 정보

	// 기타 컴포넌트들(소리와 부모 위젯)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<class USoundCue> ButtonMoveSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ParentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	bool bIsStartGameMode = true;
};
