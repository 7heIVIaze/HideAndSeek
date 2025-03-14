// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/LightItem.h"
#include "CigarLighter.generated.h"

UCLASS()
class HIDEANDSEEK_API ACigarLighter : public ALightItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACigarLighter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupLightComponent() override;

//
//public:
//	UPROPERTY(VisibleAnywhere, Category = "CigarLight")
//		TObjectPtr<USceneComponent> DefaultSceneRoot;
//
//	UPROPERTY(VisibleAnywhere, Category = "CigarLight")
//		TObjectPtr<UStaticMeshComponent> CigarLight;
//
//	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
//		TObjectPtr<class UPointLightComponent> FireLight;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
//		TObjectPtr<class UAudioComponent> CigarLightOnSound;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
//		TObjectPtr<class UAudioComponent> CigarLightOffSound;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CigarLight")
//		bool bIsCigarLightOn;*/
//
//public:
//	void OnInteract(class AHorrorGameCharacter* Player) override;
//
//	void UseInteract(class AHorrorGameCharacter* Player) override;
//
//	//UFUNCTION(BlueprintCallable)
//	//void CigarLightOn();
};
