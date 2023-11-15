// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_MoveToLocker.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"

UBTTask_MoveToLocker::UBTTask_MoveToLocker()
{
	NodeName = TEXT("MoveToLocker");
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveToLocker::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToLocker::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		Target = Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

		if (nullptr == Target)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UAIBlueprintHelperLibrary::SimpleMoveToActor(ReaperAI, Target);

		
		if (Reaper->GetAnimFinish())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		Target = Cast<AActor>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

		if (nullptr == Target)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UAIBlueprintHelperLibrary::SimpleMoveToActor(RunnerAI, Target);

		if (Runner->GetAnimFinish())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		Target = Cast<AActor>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::TargetKey));

		if (nullptr == Target)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UAIBlueprintHelperLibrary::SimpleMoveToActor(BruteAI, Target);

		if (Brute->GetAnimFinish())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

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
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if(ReaperAI)
		Target = Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

	if(RunnerAI)
		Target = Cast<AActor>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

	if (nullptr == Target)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if(ReaperAI)
		UAIBlueprintHelperLibrary::SimpleMoveToActor(ReaperAI, Target);

	if(RunnerAI)
		UAIBlueprintHelperLibrary::SimpleMoveToActor(RunnerAI, Target);

	if (Reaper)
	{
		if (Reaper->GetAnimFinish())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (Runner)
	{
		if (Runner->GetAnimFinish())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}*/
}