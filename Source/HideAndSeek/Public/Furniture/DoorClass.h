// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Resource/InteractInterface.h"
#include "Furniture/InteractableActor.h"
#include "Components/TimelineComponent.h"
#include "DoorClass.generated.h"

UCLASS()
class HIDEANDSEEK_API ADoorClass : public AInteractableActor //public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorClass();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interaction -- Left Click(Open Door)
	virtual void OnInteract(class AHorrorGameCharacter* Player) override;

	// Use Interaction -- Right Click(Use Key)
	virtual bool UseInteract(class AHorrorGameCharacter* Player) override;

	// AI Interaction -- AI Overlap.
	UFUNCTION(BlueprintCallable)
	virtual void AIInteract(class AActor* AICharacter);

	// Open Door Function
	UFUNCTION(BlueprintCallable)
	virtual void DoorOpen(float inCurveFloat);

	UFUNCTION(BlueprintCallable)
	virtual void SetDoorCollision(bool inIsPlayerNear);

	UFUNCTION(BlueprintCallable)
	virtual void PlayerBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	virtual void PlayerBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Unlock Door Function
	UFUNCTION(BlueprintCallable)
	virtual bool UnlockDoor();

	// Break Door Function
	UFUNCTION(BlueprintCallable)
	virtual void BreakDoor();

	// Change Actor Collision Function
	UFUNCTION(BlueprintCallable)
	virtual void ChangeCollisionPreset();

	// Desturction Audio Finished Callback Function.
	UFUNCTION(BlueprintCallable)
	virtual void DestructionFinished();

	// Public Variables of Door Class
public:
	// Root Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	TObjectPtr<class UBoxComponent> PlayerOverlapBox;

	// Door Mesh.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	// Destruction Audio component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Audio)
	TObjectPtr<class UAudioComponent> DestructionAudio;

	// Boolean variable to check the door is closed or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	bool bIsDoorClosed;

	// Boolean variable to check the door is locked or not.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	bool bIsDoorLocked;

	// Boolean variable to check the door is broken.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	bool bIsDoorBroken;

	// Boolean variable to check the door is breaking.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	bool bIsDoorBeingBroken;

	// Boolean variable to check whether player is near.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Variable)
	bool bIsPlayerNear;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Door")
	class AHorrorGameCharacter* PlayerCharacter;

	// The sound of the door opening
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SFX)
	TObjectPtr<class USoundCue> DoorOpenSound;

	// The sound of the door closing.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SFX)
	TObjectPtr<class USoundCue> DoorCloseSound;

	// The sound of the door being unlocked.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SFX)
	TObjectPtr<class USoundCue> DoorUnlockSound;

	// The sound of the door being broken.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = SFX)
	TObjectPtr<class USoundCue> DoorDestructionSound;

	// Timeline to implement the door opening.
	UPROPERTY(VisibleAnywhere, Category = Timeline)
	FTimeline OpenAndCloseTimeline;

	// Timeline Curve Float to implement the door opening.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Timeline)
	TObjectPtr<UCurveFloat> OpenAndCloseCurveFloat;

	// The Instance data and location of chaser.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Chaser)
	TMap<AActor*, FVector> InteractingChaser;

	// Geometry Collection Actor to implement a broken door.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Destructible)
	TSubclassOf<AActor> GC_Door;

};
