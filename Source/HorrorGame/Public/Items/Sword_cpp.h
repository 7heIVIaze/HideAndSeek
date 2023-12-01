// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Sword_cpp.generated.h"

UCLASS()
class HORRORGAME_API ASword_cpp : public AItems
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

	UPROPERTY(VisibleAnywhere, Category = "Mirror")
		class AAltar_cpp* Altar;

public:	
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;
};
