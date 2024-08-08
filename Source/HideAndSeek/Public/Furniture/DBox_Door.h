// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Resource/DoorInterface_cpp.h"
#include "DBox_Door.generated.h"

UCLASS()
class HIDEANDSEEK_API ADBox_Door : public AActor, public IDoorInterface_cpp
{
	GENERATED_BODY()
	
public:	
	ADBox_Door();

	// Sets default values for this actor's properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		TObjectPtr<USceneComponent> RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
		TObjectPtr<class UAudioComponent> OpenSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Sound)
		TObjectPtr<class UAudioComponent> CloseSound;

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
		void OpenDoor(float Value);
};
