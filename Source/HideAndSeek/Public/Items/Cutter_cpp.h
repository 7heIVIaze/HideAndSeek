// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Cutter_cpp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class HIDEANDSEEK_API ACutter_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACutter_cpp();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = "Root")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Cutter")
		UStaticMeshComponent* CutterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cutter")
		int32 Durability = 5;

public:
	// Functions
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
