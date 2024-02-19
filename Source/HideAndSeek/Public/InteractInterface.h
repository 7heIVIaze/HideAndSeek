// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com
// This interface does Item Class interact.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HIDEANDSEEK_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Get Item and Watch Item Interact Interface
	
	virtual void OnInteract(class AHorrorGameCharacter* Player) = 0;

	virtual void UseInteract(class AHorrorGameCharacter* Player) = 0;
	// void OnInteract(AHorrorGameCharacter* PlayerCharacter);

	//// Use Item Interact Interface
	//UFUNCTION(BlueprintCallable, Category = "Interaction")
	//void UseInteract(AHorrorGameCharacter* PlayerCharacter);

	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void StartFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void EndFocus();*/
};
