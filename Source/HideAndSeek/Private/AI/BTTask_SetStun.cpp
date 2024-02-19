// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_SetStun.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"

UBTTask_SetStun::UBTTask_SetStun()
{
	NodeName = TEXT("SetStun");
}

EBTNodeResult::Type UBTTask_SetStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			return EBTNodeResult::Failed;
		}

		bool stunned = Reaper->GetIsStunned();

		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, stunned);
		if (stunned)
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);

		return EBTNodeResult::Succeeded;
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}

		bool stunned = Runner->GetIsStunned();

		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::Stunned, stunned);
		if(stunned)
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);

		return EBTNodeResult::Succeeded;
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			return EBTNodeResult::Failed;
		}

		bool stunned = Brute->GetIsStunned();

		BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::Stunned, stunned);
		if(stunned)
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	/*ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	APawn* Creature = nullptr;
	AReaper_cpp* Reaper = nullptr;
	ARunner_cpp* Runner = nullptr;

	if (ReaperAI)
	{
		Creature = ReaperAI->GetPawn();
		Reaper = Cast<AReaper_cpp>(Creature);
	}

	if (RunnerAI)
	{
		Creature = RunnerAI->GetPawn();
		Runner = Cast<ARunner_cpp>(Creature);
	}

	if (nullptr == Creature)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetStun Creature initialized Failed"));
		return EBTNodeResult::Failed;
	}

	bool stunned = false;
	
	if(Reaper)
		stunned = Reaper->GetIsStunned();

	if(Runner)
		stunned = Runner->GetIsStunned();

	if(ReaperAI)
		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, stunned);

	if(RunnerAI)
		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::Stunned, stunned);
	
	return EBTNodeResult::Succeeded;*/
}