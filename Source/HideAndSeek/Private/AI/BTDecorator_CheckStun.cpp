// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTDecorator_CheckStun.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Runner.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTDecorator_CheckStun::UBTDecorator_CheckStun()
{
	NodeName = TEXT("IsStun");
}

// 조건을 판단하는 함수
bool UBTDecorator_CheckStun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	bool bStunned = false;

	if (AIController)
	{
		if (const auto ReaperAI = Cast<ACreatureAI>(AIController))
		{
			bStunned = ReaperAI->GetBlackboardComponent()->GetValueAsBool(ACreatureAI::Stunned);
		}
		else if (const auto BruteAI = Cast<AAIController_Brute>(AIController))
		{
			bStunned = BruteAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::Stunned);
		}
		else if (const auto RunnerAI = Cast<AAIController_Runner>(AIController))
		{
			bStunned = RunnerAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::Stunned);
		}
	}

	return bStunned;
}