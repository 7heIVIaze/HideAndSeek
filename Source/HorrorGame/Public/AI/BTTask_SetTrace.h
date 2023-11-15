// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_SetTrace.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HORRORGAME_API UBTTask_SetTrace : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetTrace();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
