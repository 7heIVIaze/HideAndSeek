// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "EngineMinimal.h"
#include "Items/Items.h"
#include "Key_cpp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class HIDEANDSEEK_API AKey_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKey_cpp();


public:
	UPROPERTY(VisibleAnywhere, Category = "KeyItem")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "KeyItem")
		UStaticMeshComponent* KeyMesh;

public:
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
