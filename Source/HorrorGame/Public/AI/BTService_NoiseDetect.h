// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_NoiseDetect.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTService_NoiseDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_NoiseDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
