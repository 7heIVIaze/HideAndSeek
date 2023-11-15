// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HORRORGAME_API UBTTask_FindPlayerLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPlayerLocation();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float SearchRadius = 800.0f;
//
//public:
//	// Reaper Controller And Reaper Pawn
//	class ACreatureAI* ReaperAI;
//	class AReaper_cpp* Reaper;
//
//	// Runner Controller And Runner Pawn
//	class AAIController_Runner* RunnerAI;
//	class ARunner_cpp* Runner;
//
//	// Brute Controller And Brute Pawn
//	class AAIController_Brute* BruteAI;
//	class ABrute_cpp* Brute;
};
