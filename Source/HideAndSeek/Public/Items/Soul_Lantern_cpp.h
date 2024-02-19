// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Soul_Lantern_cpp.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API ASoul_Lantern_cpp : public AItems
{
	GENERATED_BODY()
	
public:
	ASoul_Lantern_cpp();

public:
	UPROPERTY(VisibleAnywhere, Category = "Root")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		TObjectPtr<UStaticMeshComponent> LanternMesh;

	UPROPERTY(VisibleAnywhere, Category = "Light")
		TObjectPtr<class UPointLightComponent> Light;

	UPROPERTY(EditAnywhere, Category = "Light")
		TObjectPtr<class UNiagaraComponent> LightNiagara;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boolean")
		class AAltar_cpp* ObjectEnd;*/

	UPROPERTY(EditAnywhere, Category = "Boolean")
		FVector AltarLocation;

	UPROPERTY(EditAnywhere, Category = "Boolean")
		bool bIsLightOn;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundCue* LanternCue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnInteract(class AHorrorGameCharacter* Player) override;

	//UFUNCTION(BlueprintCallable)
	void UseInteract(class AHorrorGameCharacter* Player) override;


};
