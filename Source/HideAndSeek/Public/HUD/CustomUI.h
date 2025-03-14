// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/HorrorGameCharacter.h"
#include "CustomUI.generated.h"
/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UCustomUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	//UCustomUI(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	virtual void AllWidgetInit() {};

	UFUNCTION(BlueprintCallable)
	virtual void Init() {};

	UFUNCTION(BlueprintCallable)
	virtual void Refresh() {};

public:
	AHorrorGameCharacter* Player;
};
