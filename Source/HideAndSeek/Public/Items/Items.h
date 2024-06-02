// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Items.generated.h"

//class UUINavWidget;
//class UTextBlock;
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedEvent);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressedEvent);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReleasedEvent);
//
//UENUM(BlueprintType, meta = (ScriptName = "UINavButtonStyle"))
//enum class EBUttonStyle : uint8
//{
//	None UMETA(DisplayName = "None"),
//	Normal UMETA(DisplayName = "Normal"),
//	Hovered UMETA(DisplayName = "Hovered"),
//	Pressed UMETA(DisplayName = "Pressed"),
//};

UCLASS()
class HIDEANDSEEK_API AItems : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	AItems();

	UFUNCTION(BlueprintCallable)
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	virtual void UseInteract(class AHorrorGameCharacter* Player) override;
};
