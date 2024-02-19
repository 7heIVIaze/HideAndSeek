// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "CigarLighter_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ACigarLighter_cpp : public AItems
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACigarLighter_cpp();

public:
	UPROPERTY(VisibleAnywhere, Category = "CigarLight")
		TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "CigarLight")
		TObjectPtr<UStaticMeshComponent> CigarLight;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
		TObjectPtr<class UPointLightComponent> FireLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
		TObjectPtr<class UAudioComponent> CigarLightOnSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
		TObjectPtr<class UAudioComponent> CigarLightOffSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
		bool bIsCigarLightOn;*/

public:
	void OnInteract(class AHorrorGameCharacter* Player) override;

	void UseInteract(class AHorrorGameCharacter* Player) override;

	//UFUNCTION(BlueprintCallable)
	//void CigarLightOn();
};
