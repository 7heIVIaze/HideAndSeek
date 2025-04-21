// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "SwitchLever.generated.h"

UCLASS()
class HIDEANDSEEK_API ASwitchLever : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchLever();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> LeverBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Lever;

	UPROPERTY(VisibleAnywhere, Category = Lever)
	FTimeline UpAndDownTimeline; // Create TimeLine 

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundCue* LeverSound;

	UPROPERTY(EditAnywhere)
	UCurveFloat* UpAndDownCurveFloat;

	UPROPERTY()
	float LeverRotateAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lever)
	bool bIsLeverOn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	void ChangeLeverRotation(float Value);
};
