// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HIDEANDSEEK_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrolPos();


	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float SearchRadius = 1500.0f;
};
