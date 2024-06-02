// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Extinguisher_cpp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class HIDEANDSEEK_API AExtinguisher_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtinguisher_cpp();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = "Extinguisher")
		TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Extinguisher")
		TObjectPtr<UStaticMeshComponent> ExtinguisherMesh;

public:	// Functions
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
