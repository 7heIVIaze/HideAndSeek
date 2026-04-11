// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// This interface does Item Class interact.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HIDEANDSEEK_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Get Item and Watch Item Interact Interface
	
	virtual bool SetSpawnItem(TSubclassOf<AActor> inItem) = 0;
};
