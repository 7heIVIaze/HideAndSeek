// Fill out your copyright notice in the Description page of Project Settings.
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
class HORRORGAME_API AClassroomDoors_cpp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClassroomDoors_cpp();

public: // properties
	UPROPERTY(EditAnywhere, Category = "Root")
		USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, Category = "Door")
		AClassroomDoorActor_cpp* LeftDoor;

	UPROPERTY(EditAnywhere, Category = "Door")
		AClassroomDoorActor_cpp* RightDoor;

	//UPROPERTY(EditAnywhere, Category = "Door")
	//	UStaticMeshComponent* LockMeshFront;

	//UPROPERTY(EditAnywhere, Category = "Door")
	//	UStaticMeshComponent* LockMeshBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
		bool bIsDoorLocked = true;

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

	// Functions
	/*UFUNCTION()
		void ChangeValue();*/



};
