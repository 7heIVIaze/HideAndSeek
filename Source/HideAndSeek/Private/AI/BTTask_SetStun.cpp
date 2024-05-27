// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_SetStun.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"

// AI를 행동 불능 상태로 만드는 로직
UBTTask_SetStun::UBTTask_SetStun()
{
	NodeName = TEXT("SetStun");
}

EBTNodeResult::Type UBTTask_SetStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 그 컨트롤러가 Reaper의 컨트롤러라면
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			return EBTNodeResult::Failed;
		}
		
		// Reaper가 행동 불능 상태라면 컨트롤러에도 업데이트하고 Target을 초기화함
		bool stunned = Reaper->GetIsStunned();

		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, stunned);
		if (stunned)
		{
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
		}

		return EBTNodeResult::Succeeded;
	}
	// 그 컨트롤러가 Runner의 컨트롤러라면
	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}
		
		// Runner가 행동 불능 상태라면 컨트롤러에도 업데이트하고 Target을 초기화함
		bool stunned = Runner->GetIsStunned();

		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::Stunned, stunned);
		if (stunned)
		{
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
		}

		return EBTNodeResult::Succeeded;
	}
	// 그 컨트롤러가 Brute의 컨트롤러라면
	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			return EBTNodeResult::Failed;
		}
		
		// Brute가 행동 불능 상태라면 컨트롤러에도 업데이트하고 Target을 초기화함
		bool stunned = Brute->GetIsStunned();

		BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::Stunned, stunned);
		if (stunned)
		{
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}