// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTService_Stuning.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_Stuning::UBTService_Stuning()
{
	NodeName = TEXT("CheckStun");
	Interval = 0.5f;
}

void UBTService_Stuning::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	/*ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	AReaper_cpp* Reaper = nullptr;
	ARunner_cpp* Runner = nullptr;*/

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		bool bIsStunned = false;
		bool bCurrentStatus = false;

		bIsStunned = Reaper->GetIsStunned();
		bCurrentStatus = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::Stunned);
		
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