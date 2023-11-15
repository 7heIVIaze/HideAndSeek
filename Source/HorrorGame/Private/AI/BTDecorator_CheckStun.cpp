// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTDecorator_CheckStun.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Runner.h"
//#include "AI/Reaper_cpp.h"
//#include "AI/Runner_cpp.h"
//#include "AI/Brute_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckStun::UBTDecorator_CheckStun()
{
	NodeName = TEXT("IsStun");
}

bool UBTDecorator_CheckStun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	bool bStunned = false;

	if (Controller)
	{
		if (Controller->IsA(ACreatureAI::StaticClass()))
		{
			bStunned = Controller->GetBlackboardComponent()->GetValueAsBool(ACreatureAI::Stunned);
		}
		else if (Controller->IsA(AAIController_Brute::StaticClass()))
		{
			bStunned = Controller->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::Stunned);
		}
		else if (Controller->IsA(AAIController_Runner::StaticClass()))
		{
			bStunned = Controller->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::Stunned);
		}
	}

	return bStunned;
}