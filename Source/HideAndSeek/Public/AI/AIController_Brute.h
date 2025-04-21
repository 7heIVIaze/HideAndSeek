// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "YokaiAIController.h"
#include "AIController_Brute.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Brute : public AYokaiAIController //AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Brute();

	virtual void SetupBlackboardAndBehaviorTree() override;

	virtual void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus) override;

public:
	// 플레이어가 걷는 소리를 듣고 감지 범위가 늘었는지 판단할 boolean 데이터
	static const FName ChangeDetectRange;

};
