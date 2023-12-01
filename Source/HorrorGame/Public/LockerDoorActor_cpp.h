// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "LockerDoorActor_cpp.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class UAudioComponent;
UCLASS()
class HORRORGAME_API ALockerDoorActor_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALockerDoorActor_cpp();

public: // Properties
	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Locker")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerDoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerDoorFrame;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerDoorLock;

	UPROPERTY(VisibleAnywhere, Category = "Locker")
		UStaticMeshComponent* LockerLockMesh;

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Locker")
		FTimeline OpenAndClose; // Create TimeLine 

	UPROPERTY(EditAnywhere)
		UCurveFloat* CurveFloat; // Timeline Curve

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
		UAudioComponent* LockerSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
		float LockerDoorRotateAngle = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
	bool bIsLockerClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locker")
	bool bIsLockerLocked = true;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	 // Functions
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnInteract(class AHorrorGameCharacter* Player);

	UFUNCTION()
		void OpenDoor(float Value);

	UFUNCTION()
		void UseInteract(class AHorrorGameCharacter* Player);
};
