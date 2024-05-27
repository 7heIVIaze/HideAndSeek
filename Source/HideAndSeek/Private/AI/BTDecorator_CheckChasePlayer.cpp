// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTDecorator_CheckChasePlayer.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Runner.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTDecorator_CheckChasePlayer::UBTDecorator_CheckChasePlayer()
{
	NodeName = TEXT("PlayerChase");
}

// 조건을 계산하는 함수
bool UBTDecorator_CheckChasePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	bool bPlayerChase = false;

	if (AIController)
	{
		if (const auto ReaperAI = Cast<ACreatureAI>(AIController))
		{
			bPlayerChase = ReaperAI->GetBlackboardComponent()->GetValueAsBool(ACreatureAI::CanSeePlayer);
		}
		else if (const auto RunnerAI = Cast<AAIController_Runner>(AIController))
		{
			bPlayerChase = RunnerAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
		}
		else if (const auto BruteAI = Cast<AAIController_Brute>(AIController))
		{
			bPlayerChase = BruteAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
		}
	}

	return bPlayerChase;
}