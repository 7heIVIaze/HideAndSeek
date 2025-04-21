// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "LockerDoorActor_cpp.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UAudioComponent;
UCLASS()
class HIDEANDSEEK_API ALockerDoorActor_cpp : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockerDoorActor_cpp();

public: // Properties
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Locker")
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerLockMesh;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
	FTimeline OpenAndCloseTimeline; // Create TimeLine 

	UPROPERTY(EditAnywhere)
	UCurveFloat* OpenAndCloseCurveFloat; // Timeline Curve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	TObjectPtr<class USoundCue> UnlockSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr<class USoundCue> DoorCloseSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr<class USoundCue> DoorOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
	float DoorRotateAngle = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
	bool bIsDoorClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
	bool bIsDoorLocked = true;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	 // Functions
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual void DoorOpen(float Value);

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;
};
