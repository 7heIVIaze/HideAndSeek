// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HideInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UHideInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORRORGAME_API IHideInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnInteract(class AHorrorGameCharacter* PlayerCharacter) = 0;

	// virtual void OnInteract(class AHorrorGameCharacter* Player) = 0;
};
