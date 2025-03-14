// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/ActiveItem.h"
#include "FireExtinguisher.generated.h"

class USceneComponent;
class UStaticMeshComponent;
UCLASS()
class HIDEANDSEEK_API AFireExtinguisher : public AActiveItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFireExtinguisher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Use Interaction: by right click.
	//UFUNCTION(BlueprintCallable)
	virtual bool UseItem() override;
	// bool UseInteract(class APlayerCharacter* Player);

	// Stop spraying the powder
	UFUNCTION(BlueprintCallable)
	bool StopSpraying();
	// bool StopSpraying(class APlayerCharacter* Player);

	// Capsule Collision.
	UFUNCTION(BlueprintCallable)
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void SetItemVisibility(bool NewVisibility) override;

	////////////////
	// PROPERTIES //
	////////////////
public:
	/*UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> ExtinguisherMesh;*/

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TObjectPtr<class UParticleSystemComponent> PowderParticle;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	TObjectPtr<class UCapsuleComponent> PowderCapsule;

	// Spraying sound
	UPROPERTY(EditDefaultsOnly, Category = Light)
	class USoundCue* SprayCue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Boolean)
	bool bIsSpraying;

//	
//public:	
//	// Sets default values for this actor's properties
//	AFireExtinguisher();
//
//public: // Properties
//	UPROPERTY(VisibleAnywhere, Category = "Extinguisher")
//		TObjectPtr<USceneComponent> DefaultSceneRoot;
//
//	UPROPERTY(VisibleAnywhere, Category = "Extinguisher")
//		TObjectPtr<UStaticMeshComponent> ExtinguisherMesh;
//
//public:	// Functions
//	void OnInteract(class AHorrorGameCharacter* Player) override;
//
//	void UseInteract(class AHorrorGameCharacter* Player) override;
};
