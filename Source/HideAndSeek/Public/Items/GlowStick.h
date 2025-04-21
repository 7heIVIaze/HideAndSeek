// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ConsumableItemClass.h"
#include "GlowStick.generated.h"

/*
 * 빛을 내는 아이템. 길 찾는 용도로 사용가능하도록 설정할 것 
 */
UCLASS()
class HIDEANDSEEK_API AGlowStick : public AConsumableItemClass
{
	GENERATED_BODY()
	
public:
	AGlowStick();

public: // Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	class USoundCue* UseSound;

	UPROPERTY(VisibleAnywhere, Category = Light)
	TObjectPtr<class UPointLightComponent> GlowLightComp;

public: // Functions
	UFUNCTION(BlueprintCallable)
	void SetLightOn();

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;
};
