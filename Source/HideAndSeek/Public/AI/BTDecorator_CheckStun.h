// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckStun.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTDecorator_CheckStun : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckStun();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
