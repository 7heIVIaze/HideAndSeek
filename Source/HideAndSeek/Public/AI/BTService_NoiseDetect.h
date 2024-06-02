// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_NoiseDetect.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API UBTService_NoiseDetect : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_NoiseDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
