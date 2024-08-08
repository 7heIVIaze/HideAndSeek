// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightSwitch.generated.h"

UCLASS()
class HIDEANDSEEK_API ALightSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightSwitch();

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		TObjectPtr<UStaticMeshComponent> MainMesh;

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		TObjectPtr<UStaticMeshComponent> SwitchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
		TArray<class ASchoolLight*> Lights;

	UPROPERTY(VisibleAnywhere, Category = "Switch")
		bool bIsSwitchOn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* LightSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	/*virtual void Tick(float DeltaTime) override;*/
	UFUNCTION(BlueprintCallable)
		void OnInteract();

	UFUNCTION(BlueprintCallable)
		void SetSwitchStatus();

	UFUNCTION(BlueprintCallable)
		void SetLightsStatus();
};
