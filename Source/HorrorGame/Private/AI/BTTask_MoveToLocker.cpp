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
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToLocker::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr) // AI 컴포넌트의 Controller가 없다면 실패를 리턴시킨다.
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress; // 동작 수행은 Tick Task를 통해 관리하므로 InProgress를 리턴시켜 아직 수행 중임을 알림.
}

void UBTTask_MoveToLocker::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController) // AI Controller가 있는 경우에만 아래의 동작 수행
	{
		if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
		{
			AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

			if (nullptr == Reaper)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			Target = Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

			if (nullptr == Target) // 타겟이 없으면 Fail 리턴시키고 task 종료시킴
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			ReaperAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

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

			RunnerAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

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

			BruteAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			if (Brute->GetAnimFinish())
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
	}
	else // AI Controller가 없으면 Fail을 리턴
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
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