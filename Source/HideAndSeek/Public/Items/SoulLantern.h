// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/LightItem.h"
#include "SoulLantern.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API ASoulLantern : public ALightItem
{
	GENERATED_BODY()
	
public:
	ASoulLantern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupLightComponent() override;

public:
	virtual void Tick(float DeltaTime) override;
//
//public:
//	UPROPERTY(VisibleAnywhere, Category = "Root")
//		TObjectPtr<USceneComponent> RootComp;
//
//	UPROPERTY(VisibleAnywhere, Category = "Mesh")
//		TObjectPtr<UStaticMeshComponent> LanternMesh;
//
//	UPROPERTY(VisibleAnywhere, Category = "Light")
//		TObjectPtr<class UPointLightComponent> Light;
//
//	UPROPERTY(EditAnywhere, Category = "Light")
//		TObjectPtr<class UNiagaraComponent> LightNiagara;
//
//	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boolean")
//		class AAltar_cpp* ObjectEnd;*/
//
//	UPROPERTY(EditAnywhere, Category = "Boolean")
//		FVector AltarLocation;
//
//	UPROPERTY(EditAnywhere, Category = "Boolean")
//		bool bIsLightOn;
//
//	UPROPERTY(EditDefaultsOnly, Category = "Sound")
//		class USoundCue* LanternCue;
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//	void OnInteract(class AHorrorGameCharacter* Player) override;
//
//	//UFUNCTION(BlueprintCallable)
//	void UseInteract(class AHorrorGameCharacter* Player) override;
//
//	UFUNCTION(BlueprintCallable)
//	void HideInteract(class AHorrorGameCharacter* Player);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HP")
	float HPLoss;

	UPROPERTY(BlueprintReadWrite, Category = "HP")
	float HPLossRate;
};
