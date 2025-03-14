// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HUD/CustomUI.h"
#include "ObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UObjectWidget : public UCustomUI
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

//public:
//	UFUNCTION()
//		void SetObjectCount(const unsigned int idx, int32 value);
//
//private:
//	UFUNCTION()
//		void SetSwordCount(int32 value);
//
//	UFUNCTION()
//		void SetBellCount(int32 value);
//
//	UFUNCTION()
//		void SetMirrorCount(int32 value);

public:
	virtual void Init() override;

	// 오브젝트를 얻을 때 리프레시할 함수와 델리게이트를 바인딩해주는 함수.
	UFUNCTION(BlueprintCallable)
	bool InitializeWidget(class UInventoryComponent* InventoryComponent);

private:
	// 오브젝트를 얻을 때 리프레시할 함수.
	UFUNCTION(BlueprintCallable)
	void RefreshObjectCount(int32 ObjectCount);

//protected:
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		class UTextBlock* SwordCount;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		class UTextBlock* BellCount;
//
//	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
//		class UTextBlock* MirrorCount;

public:
// Object images not yet collected
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Image)
	TObjectPtr<class UTexture2D>NotCollectedObject;

	// Collected object images
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Image)
	TObjectPtr<class UTexture2D>CollectedObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Image)
	TArray<TObjectPtr<class UImage>>ObjectImg;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> ObjectImg1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> ObjectImg2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> ObjectImg3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> ObjectImg4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> ObjectImg5;

	// Number of objects to collect.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ObjectCount)
	int ObjectNumber;
	//FString BaseText;
};