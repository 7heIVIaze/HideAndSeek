// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Components/AudioComponent.h"
#include "DoorInterface_cpp.h"
#include "MetalDoor_cpp.generated.h"

UCLASS()
class HORRORGAME_API AMetalDoor_cpp : public AActor, public IDoorInterface_cpp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetalDoor_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION()
		virtual void UseInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION()
		void AIInteract();

	UFUNCTION()
		void OpenDoor(float Value);

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
