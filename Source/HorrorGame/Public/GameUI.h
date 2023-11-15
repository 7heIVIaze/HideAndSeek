// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "GameUI.generated.h"

class UInventory_cpp;
/**
 * 
 */
UCLASS()
class HORRORGAME_API UGameUI : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
//	UGameUI(const FObjectInitializer& ObjectInitializer);


	void AllWidgetInit() override;

	void Init() override;

	UFUNCTION()
		void SetInteractDot(bool Value);

	UFUNCTION()
		void SetInteractDotText(FString strText);

	UFUNCTION()
		void SetInteractDotErrorText(FString strText);

	UFUNCTION()
		void SetInteractDotExplainText(FString strText);

	UFUNCTION()
		class UInteractDot_cpp* GetInteractDotWidget() const;

	//UFUNCTION()
	//	void SetBaseInterface(bool Value);

	UFUNCTION()
		void SetStaminaHUD(int32 iValue);

	UFUNCTION()
		void SetBatteryHUD(int32 iValue);

	UFUNCTION()
		void SetBatteryWidget(bool value);

	UFUNCTION()
		void SetCutterHUD(int32 iValue);

	UFUNCTION()
		void SetCutterWidget(bool value);

	UFUNCTION()
		void SetExtHUD(int32 value);

	UFUNCTION()
		void SetExtWidget(bool value);

	UFUNCTION()
		void SetObjectCount(unsigned int idx, int32 value); // idx: 1(Sword), 2(Bell), 3(Mirror)

	UFUNCTION()
		void SetPatience(int32 value);

	//UFUNCTION()
	//	void SetObjWidget(bool value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget))
		UInventory_cpp* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaminaWidget* StaminaWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCutterWidget* CutterWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UInteractDot_cpp* InteractDotState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UBatteryWidget_cpp* BatteryWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UExtinguisherWidget* ExtWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UObjectWidget* ObjWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UExplainWidget_cpp* ExpWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPatienceWidget* PatienceWidget;

private:
	bool BatteryVisible = false;
	bool CutterVisible = false;
	bool ExtVisible = false;
	//bool ObjVisible = true;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UDiedWidget* DiedWidget;*/
	
};
