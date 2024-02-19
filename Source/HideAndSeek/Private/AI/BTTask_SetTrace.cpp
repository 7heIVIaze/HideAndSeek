// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_SetTrace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"

UBTTask_SetTrace::UBTTask_SetTrace()
{
	NodeName = TEXT("SetTrace");
}

EBTNodeResult::Type UBTTask_SetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

		if (nullptr == pTarget)
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
			// ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
			Reaper->EndChase();
		}

		else
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, true);
			Reaper->StartChase();
		}

		bool bNoiseDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);

		if (bNoiseDetected)
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, false);
		}

		return EBTNodeResult::Succeeded;
	}

	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}

		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

		if (nullptr == pTarget)
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
			// RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
			Runner->EndChase();
		}

		else
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, true);
			Runner->StartChase();
		}

		bool bNoiseDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected);

		if (bNoiseDetected)
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, false);
		}

		return EBTNodeResult::Succeeded;
	}

	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			return EBTNodeResult::Failed;
		}

		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::TargetKey));

		if (nullptr == pTarget)
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, false);
			// BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
			Brute->EndChase();
		}

		else
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, true);
			Brute->StartChase();
		}

		bool bNoiseDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::NoiseDetected);

		if (bNoiseDetected)
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, false);
			Brute->ChangeNoiseRange(false);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
	//ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	//AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	//APawn* Creature = nullptr;
	//AReaper_cpp* Reaper = nullptr;
	//ARunner_cpp* Runner = nullptr;
	//bool bNoiseDetected = false;
	//
	//if (ReaperAI)
	//{
	//	Creature = ReaperAI->GetPawn();
	//	Reaper = Cast<AReaper_cpp>(Creature);
	//}
	//
	//if (RunnerAI)
	//{
	//	Creature = RunnerAI->GetPawn();
	//	Runner = Cast<ARunner_cpp>(Creature);
	//}
	//
	//if (nullptr == Creature)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("SetTrace Creature Initialized Failed"));
	//	return EBTNodeResult::Failed;
	//}
	//
	//AHorrorGameCharacter* pTarget = nullptr;
//
	//if(ReaperAI)
	//	pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));
	//
	//if(RunnerAI)
	//	pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));
	////bool bCanSeePlayer = AIController->GetBlackboard()->GetValueAsBool(ACreatureAI::CanSeePlayer);
	//if (nullptr == pTarget)
	//{
	//	if (ReaperAI)
	//	{
	//		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
	//		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
	//		Reaper->EndChase();
	//	}
	//
	//	if (RunnerAI)
	//	{
	//		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
	//		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
	//		Runner->EndChase();
	//	}
	//}
	//
	//else
	//{
	//	if (ReaperAI)
	//	{
	//		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, true);
	//		Reaper->StartChase();
	//	}
	//	
	//	if (RunnerAI)
	//	{
	//		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, true);
	//		Runner->StartChase();
	//	}
	//}
	//
	//if(ReaperAI)
	//	bNoiseDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);
	//
	//if(RunnerAI)
	//	bNoiseDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected);
	//
	//if (bNoiseDetected)
	//{
	//	if(ReaperAI)
	//		ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, false);
	//
	//	if(RunnerAI)
	//		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, false);
	//}
	//
	//return EBTNodeResult::Succeeded;
}