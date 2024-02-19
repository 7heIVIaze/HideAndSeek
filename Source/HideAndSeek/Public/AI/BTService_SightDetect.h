// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SightDetect.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTService_SightDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SightDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Chase")
		float FlashLightDist;

	UPROPERTY(EditAnywhere, Category = "Chase")
		float CigarLightDist;

	UPROPERTY(EditAnywhere, Category = "Chase")
		float NoLightDist;
};
