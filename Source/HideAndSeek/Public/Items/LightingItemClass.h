// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemClass.h"
#include "LightingItemClass.generated.h"

UCLASS()
class HIDEANDSEEK_API ALightingItemClass : public AItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightingItemClass();

public:	
	// 상호작용 함수 -- 좌클릭
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	// 아이템 사용 함수 -- 우클릭
	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	// 상호작용(길게) 함수 -- 좌클릭 길게
	virtual void HoldInteract(class AHorrorGameCharacter* Player) override;

	virtual FHorrorGameItemData GetItemData() override;
};
