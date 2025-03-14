// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Components/TimelineComponent.h"
//#include "Components/AudioComponent.h"
//#include "Resource/DoorInterface_cpp.h"
#include "Furniture/DoorClass.h"
#include "MetalDoor_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API AMetalDoor_cpp : public ADoorClass //public AActor, public IDoorInterface_cpp
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

	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	virtual void AIInteract(AActor* Creature) override;

	virtual void DoorOpen(float Value) override;

	//UFUNCTION()
	//	void SetDoorCollision(bool inIsPlayerNear);

	//UFUNCTION()
	//	void PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//UFUNCTION()
	//	void PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ChangeCollisionPreset() override;

	virtual void BreakDoor() override;

	virtual void DestructionFinished() override;

	/*UFUNCTION()
		void CreatureBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

public:
	//UPROPERTY(VisibleAnywhere, Category = "Door")
	//	TObjectPtr<UStaticMeshComponent> Door;

	///** Please add a variable description */
	//UPROPERTY(VisibleAnywhere, Category = "Door")
	//	TObjectPtr<USceneComponent> DefaultSceneRoot;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TObjectPtr<class UBoxComponent> PlayerOverlapBox;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TObjectPtr<class UAudioComponent> DestructionAudio;

	///*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//	TObjectPtr<class UBoxComponent> CreatureOverlapBox;*/

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
	//bool bIsDoorLocked = true;

	//UPROPERTY(BlueprintReadWrite)
	//bool bIsDoorBroken = false;

	//UPROPERTY(BlueprintReadWrite)
	//bool bIsPlayerNear = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//bool bIsDoorBreaking = false;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundCue* DoorOpenSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundCue* DoorCloseSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	class USoundCue* DoorBreakSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	TMap<AActor*, FVector> InteractingCreatures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	TSubclassOf<class AActor> GC_Door;*/

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	//class AHorrorGameCharacter* PlayerCharacter;
};
