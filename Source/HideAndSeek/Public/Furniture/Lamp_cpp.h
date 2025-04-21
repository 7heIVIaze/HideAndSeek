// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Furniture/LightingClass.h"
#include "Components/TimelineComponent.h"
#include "Lamp_cpp.generated.h"

UCLASS()
class HIDEANDSEEK_API ALamp_cpp : public ALightingClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALamp_cpp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void LightFlicker(float value) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightBoolean")
		bool bIsNaturalFlick;

	UPROPERTY(EditAnywhere, Category = "LightAnim")
	UCurveFloat* NaturalFlickeringCurveFloat;

	float Intensity = 8000.f;
};
