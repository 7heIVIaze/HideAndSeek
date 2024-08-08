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

public:
	UFUNCTION()
		void SetObjectCount(const unsigned int idx, int32 value);

private:
	UFUNCTION()
		void SetSwordCount(int32 value);

	UFUNCTION()
		void SetBellCount(int32 value);

	UFUNCTION()
		void SetMirrorCount(int32 value);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* SwordCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* BellCount;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* MirrorCount;

	FString BaseText;
};