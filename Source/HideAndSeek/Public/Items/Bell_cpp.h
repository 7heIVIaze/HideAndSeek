// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemClass.h"
#include "Bell_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ABell_cpp : public AItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABell_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public: // UPROPERTY
	UPROPERTY(VisibleAnywhere, Category = "Altar")
		class AAltar_cpp* Altar;

	UPROPERTY(EditAnywhere, Category = "Sound")
		class USoundCue* ObjectSound;

public: // UFUNCTION
	void OnInteract(class AHorrorGameCharacter* Player) override;

	bool UseInteract(class AHorrorGameCharacter* Player) override;
};
