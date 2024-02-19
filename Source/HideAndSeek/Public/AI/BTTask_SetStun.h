// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_SetStun.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HIDEANDSEEK_API UBTTask_SetStun : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetStun();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
