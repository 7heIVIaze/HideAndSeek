// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemClass.h"
#include "PassiveItemClass.generated.h"


UCLASS()
class HIDEANDSEEK_API APassiveItemClass : public AItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APassiveItemClass();

public:
	// Interact Function -- Left Click
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	// Use Interact Function -- Right Click
	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	// Interact Function -- Left Click Hold
	virtual void HoldInteract(class AHorrorGameCharacter* Player);

	virtual FHorrorGameItemData GetItemData() override;

};
