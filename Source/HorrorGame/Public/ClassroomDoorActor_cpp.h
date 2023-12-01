// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "DoorInterface_cpp.h"
#include "ClassroomDoorActor_cpp.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UAudioComponent;
UCLASS()
class HORRORGAME_API AClassroomDoorActor_cpp : public AActor, public IDoorInterface_cpp
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassroomDoorActor_cpp();

public: // Properties
	UPROPERTY(VisibleAnywhere, Category = "Door")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Door")
		UStaticMeshComponent* DoorMesh;

	UPROPERTY(VisibleAnywhere, Category = "DoorLock")
		UStaticMeshComponent* Lock;

	/*UPROPERTY(VisibleAnywhere, AIInteract)
		class UBoxComponent* BoxComp;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		float DoorMovement = 150.0f;

	/** Please add a variable description */
	UPROPERTY(VisibleAnywhere, Category = "Door")
		FTimeline OpenAndClose; // Create TimeLine 

	UPROPERTY(EditAnywhere)
		class UCurveFloat* CurveFloat; // Timeline Curve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsDoorClosed = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsOtherDoorClosed = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsDoorLocked = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bNearPlayer = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Functions

	UFUNCTION()
		virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION()
		void AIInteract();

	UFUNCTION()
		virtual void UseInteract(class AHorrorGameCharacter* Player) override;

	UFUNCTION()
		void DoorOpen(float Value);

	UFUNCTION()
		void SetDoorUnlock();

	UFUNCTION()
		void SetKnowOtherDoorOpen(bool value);

	UFUNCTION()
		void SetDoorCollision(bool value);
private:
	FVector ClosedLoc;
	FVector OpenLoc;

	//bool bAdjacentDoorLocked = false;

//private:
//	UFUNCTION()
//		void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
