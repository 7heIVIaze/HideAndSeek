// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTDecorator_CheckChasePlayer.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Runner.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckChasePlayer::UBTDecorator_CheckChasePlayer()
{
	NodeName = TEXT("PlayerChase");
}

bool UBTDecorator_CheckChasePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	bool bPlayerChase = false;

	if (AIController)
	{
		if (AIController->IsA(ACreatureAI::StaticClass()))
		{
			bPlayerChase = AIController->GetBlackboardComponent()->GetValueAsBool(ACreatureAI::CanSeePlayer);
		}
		else if (AIController->IsA(AAIController_Brute::StaticClass()))
		{
			bPlayerChase = AIController->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
		}
		else if (AIController->IsA(AAIController_Runner::StaticClass()))
		{
			bPlayerChase = AIController->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
		}
	}

	return bPlayerChase;
}