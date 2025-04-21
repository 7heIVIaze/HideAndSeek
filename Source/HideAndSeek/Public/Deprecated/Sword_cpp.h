// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Deprecated/Items.h"
#include "Sword_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ASword_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Root")
		class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Sword")
		TObjectPtr<UStaticMeshComponent> Sword;

	UPROPERTY(VisibleAnywhere, Category = "Altar")
		class AAltar_cpp* Altar;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundCue* ObjectSound;


public:	
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;
};
