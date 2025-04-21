// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "YokaiAIController.h"
#include "AIController_Shadow.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Shadow : public AYokaiAIController //AAIController
{
	GENERATED_BODY()
	
public:
	// Constructor
	AAIController_Shadow();

	virtual void SetupBlackboardAndBehaviorTree() override;

	virtual void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus) override;

};
