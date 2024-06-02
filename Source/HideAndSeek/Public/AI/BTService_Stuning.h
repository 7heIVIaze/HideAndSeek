// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Stuning.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTService_Stuning : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_Stuning();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
