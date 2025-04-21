// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "YokaiAIController.h"
#include "AIController_Rampage.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Rampage : public AYokaiAIController //AAIController
{
	GENERATED_BODY()

public:
	AAIController_Rampage();

	virtual void SetupBlackboardAndBehaviorTree() override;

public:
	static const FName IsCinematic;
	static const FName CalledByBerith;

};
