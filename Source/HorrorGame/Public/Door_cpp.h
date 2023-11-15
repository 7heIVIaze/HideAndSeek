// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "DoorInterface_cpp.h"
#include "Door_cpp.generated.h"

UCLASS()
class HORRORGAME_API ADoor_cpp : public AActor, public IDoorInterface_cpp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnInteract() override;

	UFUNCTION()
		virtual void UseInteract() override;

	UFUNCTION()
		void OpenDoor(float Value);

	UFUNCTION()
		void SetDoorCollision(bool value);

public:
	UPROPERTY(VisibleAnywhere, Category = "Door")
		TObjectPtr<UStaticMeshComponent> Door;

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Door")
		TObjectPtr<USceneComponent> DefaultSceneRoot;
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Door")
		FTimeline OpenAndClose; // Create TimeLine 

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat; // Timeline Curve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		float DoorRotateAngle = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		bool bIsDoorClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		bool bIsDoorLocked = true;
};
