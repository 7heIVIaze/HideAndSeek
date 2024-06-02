// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// This Actor is just manage boolean value of ClassroomDoor_cpp s. 
// If a bIsDoorLocked value of One of ClassroomDoor_cpps is false, this actor make the other's value false.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClassroomDoors_cpp.generated.h"

class USceneComponent;
class AClassroomDoorActor_cpp;
class UStaticMeshComponent;
UCLASS()
class HIDEANDSEEK_API AClassroomDoors_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassroomDoors_cpp();

public: // properties
	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Door")
		TObjectPtr<UChildActorComponent> LeftDoorActor;

	UPROPERTY(EditAnywhere, Category = "Door")
		TObjectPtr<UChildActorComponent> RightDoorActor;

	UPROPERTY(EditAnywhere, Category = "Door")
		AClassroomDoorActor_cpp* LeftDoor;

	UPROPERTY(EditAnywhere, Category = "Door")
		AClassroomDoorActor_cpp* RightDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
		TObjectPtr<class UBoxComponent> PlayerOverlapBox;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
		class USoundCue* DoorBreakSound;

	UPROPERTY(EditAnywhere, Category = "Lock")
		bool bIsLocked;

	UPROPERTY(EditAnywhere, Category = "Lock")
		bool bIsDoorClose = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		bool bNearPlayer = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//FTimerHandle _BooleanCheckTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetDoorCollision(bool value);

	UFUNCTION()
		void SetDoorUnlock();

	UFUNCTION()
		void SetKnowOtherDoorOpen(bool value);

	UFUNCTION()
		void PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*UFUNCTION()
		void CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	UFUNCTION()
		void BreakDoor();
	// Functions
	/*UFUNCTION()
		void ChangeValue();*/



};
