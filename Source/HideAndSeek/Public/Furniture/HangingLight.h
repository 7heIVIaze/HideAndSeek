// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Furniture/LightingClass.h"
#include "HangingLight.generated.h"

UCLASS()
class HIDEANDSEEK_API AHangingLight : public ALightingClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHangingLight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void LightFlicker(float value) override;
};
