// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/DoorClass.h"
#include "DoorSlide.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UAudioComponent;
UCLASS()
class HIDEANDSEEK_API ADoorSlide : public ADoorClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorSlide();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = "DoorLock")
	TObjectPtr<UStaticMeshComponent> Lock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float DoorMovement = 150.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsOtherDoorClosed = true;

	UPROPERTY(BlueprintReadWrite)
	class AClassroomDoors_cpp* CD_Manager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Functions
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	virtual void AIInteract(AActor* AICharacter) override;

	virtual void DoorOpen(float inCurveFloat) override;

	virtual void ChangeCollisionPreset() override;

	virtual void BreakDoor() override;

	virtual void DestructionFinished() override;
	
	UFUNCTION(BlueprintCallable)
	void SetDoorUnlock();

	UFUNCTION(BlueprintCallable)
	void SetKnowOtherDoorOpen(bool value);

	UPROPERTY()
		FTimerHandle Timer;

private:
	FVector ClosedLoc;
	FVector OpenLoc;
};
