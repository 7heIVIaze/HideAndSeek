// CopyrightNotice=0 2023 Sunggon Kim

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_MoveToLocker.generated.h"

/**
 * 
 */
UCLASS()
class HORRORGAME_API UBTTask_MoveToLocker : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToLocker();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	AActor* Target = nullptr;
};
