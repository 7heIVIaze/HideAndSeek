// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "MessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UMessageWidget : public UCustomUI
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetMessage(FText Message, float Duration);

	UFUNCTION(BlueprintCallable)
	void RemoveWidget();

	UFUNCTION(BlueprintCallable)
	void SetParentWidget(class UMessageManagerWidget* ManagerWidget);

	UFUNCTION(BlueprintCallable)
	void StartFadeout();

	UFUNCTION(BlueprintCallable)
	void FadeOutFinished();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock>MessageText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation>FadeoutAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Event)
	FWidgetAnimationDynamicEvent FadeoutAnimEvent;

private:
	UPROPERTY()
	float Timer;

	UPROPERTY()
	float MessageDuration;

	UPROPERTY()
	bool bIsMessageOn;

	UPROPERTY()
	TObjectPtr<class UMessageManagerWidget> Manager;
};
