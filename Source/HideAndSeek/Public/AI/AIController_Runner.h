// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "YokaiAIController.h"
#include "AIController_Runner.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Runner : public  AYokaiAIController //AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Runner();

	virtual void SetupBlackboardAndBehaviorTree() override;
	
	virtual void SetPerception() override;
	
};
