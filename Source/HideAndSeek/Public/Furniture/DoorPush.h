// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/DoorClass.h"
//#include "GameFramework/Actor.h"
//#include "Components/TimelineComponent.h"
//#include "Components/AudioComponent.h"
//#include "Resource/DoorInterface_cpp.h"
#include "DoorPush.generated.h"

UCLASS()
class HIDEANDSEEK_API ADoorPush : public ADoorClass //public AActor, public IDoorInterface_cpp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoorPush();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	virtual void AIInteract(AActor* AICharacter) override;

	virtual void DoorOpen(float inCurveFloat) override;

	/*virtual void SetDoorCollision(bool inIsPlayerNear) override;

	virtual void PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;*/

	virtual void ChangeCollisionPreset() override;

	virtual void BreakDoor() override;

	virtual void DestructionFinished() override;

	/*UFUNCTION()
		void CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

//public:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TObjectPtr<UStaticMeshComponent> Door;

	///** Please add a variable description */
	//UPROPERTY(VisibleAnywhere, Category = "Door")
	//	TObjectPtr<USceneComponent> DefaultSceneRoot;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//TObjectPtr<class UBoxComponent> PlayerOverlapBox;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TObjectPtr<class UAudioComponent> DestructionAudio;

	///** Please add a variable description */
	//UPROPERTY(VisibleAnywhere, Category = "Door")
	//	FTimeline OpenAndClose; // Create TimeLine 

	//UPROPERTY(EditAnywhere)
	//	UCurveFloat* CurveFloat; // Timeline Curve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float DoorRotateAngle = 90.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	//	bool bIsDoorClosed = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	//	bool bIsDoorLocked = true;

	//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	bool bIsDoorBroken = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	bool bIsDoorBreaking = false;

	//	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	bool bIsPlayerNear = false;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	//	class USoundCue* DoorOpenSound;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	//	class USoundCue* DoorCloseSound;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	//	class USoundCue* DoorBreakSound;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TMap<AActor*, FVector> InteractingCreatures;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	//TSubclassOf<class AActor> GC_Door;
	//
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//class AHorrorGameCharacter* PlayerCharacter;
	//	
	//UPROPERTY()
	//	FTimerHandle Timer;
};
