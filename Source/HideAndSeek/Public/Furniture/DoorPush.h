// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/DoorClass.h"
#include "DoorPush.generated.h"

UCLASS()
class HIDEANDSEEK_API ADoorPush : public ADoorClass
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

	virtual void ChangeCollisionPreset() override;

	virtual void BreakDoor() override;

	virtual void DestructionFinished() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float DoorRotateAngle = 90.0f;

};
