// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Items/LightItem.h"
#include "SoulLantern.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API ASoulLantern : public ALightItem
{
	GENERATED_BODY()
	
public:
	ASoulLantern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupLightComponent() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HP")
	float HPLoss;

	UPROPERTY(BlueprintReadWrite, Category = "HP")
	float HPLossRate;
};
