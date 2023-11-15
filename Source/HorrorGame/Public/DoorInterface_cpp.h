// All Door Class Interact

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DoorInterface_cpp.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDoorInterface_cpp : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORGAME_API IDoorInterface_cpp
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnInteract() = 0; // Door On Click Interact Function
	virtual void UseInteract() = 0; // Door Item Use Interact Function
};
