// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/LightItem.h"
#include "FlashLight.generated.h"

UCLASS()
class HIDEANDSEEK_API AFlashLight : public ALightItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlashLight();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupLightComponent() override;
};
