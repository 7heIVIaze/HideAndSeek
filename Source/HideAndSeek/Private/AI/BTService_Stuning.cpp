// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTService_Stuning.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"

// 행동 불능 상태에 빠졌는지 체크할 BT 서비스
UBTService_Stuning::UBTService_Stuning()
{
	NodeName = TEXT("CheckStun");
	Interval = 0.5f;
}

void UBTService_Stuning::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	// 이 서비스를 가지고 있는 AI 컨트롤러를 가져옴
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	
	// 그 컨트롤러가 Reaper의 AI라면
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		// AI에 있는 현재 상태를 가져옴
		bool bCurrentStatus = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::Stunned);

		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		bool bIsStunned = false;
		if (Reaper)
		{
			bIsStunned = Reaper->GetIsStunned();
		}
		
		if (bIsStunned != bCurrentStatus)
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, bIsStunned);
		}
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		bool bIsStunned = false;
		bool bCurrentStatus = false;
		
		bIsStunned = Runner->GetIsStunned();
		bCurrentStatus = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::Stunned);
		
		if (bIsStunned != bCurrentStatus)
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::Stunned, bIsStunned);
		}
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		bool bIsStunned = false;
		bool bCurrentStatus = false;

		bIsStunned = Brute->GetIsStunned();
		bCurrentStatus = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::Stunned);
		
		if (bIsStunned != bCurrentStatus)
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::Stunned, bIsStunned);
		}
	}

	/*if(ReaperAI)
		Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

	if(RunnerAI)
		Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

	bool bIsStunned = false;
	bool bCurrentStatus = false;
	
	if (Reaper)
	{
		bIsStunned = Reaper->GetIsStunned();
		bCurrentStatus = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::Stunned);
	}

	if (Runner)
	{
		bIsStunned = Runner->GetIsStunned();
		bCurrentStatus = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::Stunned);
	}

	if (bIsStunned != bCurrentStatus)
	{
		if(ReaperAI)
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::Stunned, bIsStunned);

		if(RunnerAI)
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::Stunned, bIsStunned);
	}*/
}