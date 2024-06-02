// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTDecorator_CheckChasePlayer : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckChasePlayer();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
