// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UGameUI : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
//	UGameUI(const FObjectInitializer& ObjectInitializer);
	void AllWidgetInit() override;

	void Init() override;

	UFUNCTION(BlueprintCallable)
	bool InitializeInventoryWidget(class UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	bool InitializeObjectWidget(class UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	bool InitializeStatWidget(class UPlayerStatComponent* StatComponent);

	UFUNCTION(BlueprintCallable)
	void SetInteractDot(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetInteractDotText(FText strText);

	UFUNCTION(BlueprintCallable)
	void SetInteractDotErrorText(FText strText);

	UFUNCTION(BlueprintCallable)
	void SetInteractDotExplainText(FText strText);

	UFUNCTION(BlueprintCallable)
	class UInteractDot_cpp* GetInteractDotWidget() const;

	UFUNCTION(BlueprintCallable)
	class UTimerWidget* GetTimerWidget() const;

	//UFUNCTION(BlueprintCallable)
	//	void SetBaseInterface(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetStaminaHUD(int32 iValue);

	//UFUNCTION(BlueprintCallable)
	//void SetBatteryHUD(int32 iValue);

	//UFUNCTION(BlueprintCallable)
	//void SetBatteryWidget(bool value);

	/*UFUNCTION(BlueprintCallable)
	void SetCutterHUD(int32 iValue);

	UFUNCTION(BlueprintCallable)
	void SetCutterWidget(bool value);

	UFUNCTION(BlueprintCallable)
	void SetExtHUD(int32 value);

	UFUNCTION(BlueprintCallable)
	void SetExtWidget(bool value);*/

	//UFUNCTION(BlueprintCallable)
	//	void SetObjectCount(unsigned int idx, int32 value); // idx: 1(Sword), 2(Bell), 3(Mirror)

	UFUNCTION(BlueprintCallable)
	void SetPatience(int32 value);

	UFUNCTION(BlueprintCallable)
	void SetTimerWidget(bool value);

	UFUNCTION(BlueprintCallable)
	void SetTimerStop(bool inStop);

	UFUNCTION(BlueprintCallable)
	void SetCrossHairVisible(bool value);

	UFUNCTION(BlueprintCallable)
	void SetArchiveGetText(FText inText);

	UFUNCTION(BlueprintCallable)
	void ShowMessage(FText Message);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	TObjectPtr<class UInventoryWidget> InventoryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	TObjectPtr<class UStaminaWidget> StaminaWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	TObjectPtr<class UMessageManagerWidget> MessageWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	TObjectPtr<class UInteractDot_cpp> InteractDotState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	TObjectPtr<class UStatWidget> StatWidget;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
	//	class UExtinguisherWidget* ExtWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
		class UObjectWidget* ObjectWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
		class UExplainWidget_cpp* ExpWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (BindWidget))
		class UPatienceWidget* PatienceWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTimerWidget* TimerWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UCrosshairWidget* Crosshair;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UTextBlock* ArchiveGetText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UOverlay* MainInterface;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UUserWidget* LoadingWidget;*/

//private:
//	bool BatteryVisible = false;
//	bool CutterVisible = false;
//	bool ExtVisible = false;
	//bool CrossVisible = true;
	//bool InteractVisible = false;
	//bool DotVisible = false;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDiedWidget* DiedWidget;*/
	
};
