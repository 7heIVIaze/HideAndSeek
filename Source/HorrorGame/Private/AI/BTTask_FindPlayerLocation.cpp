// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_FindPlayerLocation.h"
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

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = TEXT("FindPlayerLocation");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		UWorld* World = Reaper->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}

		UWorld* World = Runner->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			return EBTNodeResult::Failed;
		}

		UWorld* World = Brute->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
	//ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	//AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	//APawn* Creature = nullptr;
	///*AReaper_cpp* Reaper = nullptr;
	//ARunner_cpp* Runner = nullptr;*/
	//
	//if(ReaperAI)
	//	Creature = ReaperAI->GetPawn();
	//
	//if(RunnerAI)
	//	Creature = RunnerAI->GetPawn();
	//
	//// auto Reaper = Cast<AReaper_cpp>(Creature);
	//if (nullptr == Creature)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Creature Initialize Failed!"));
	//	return EBTNodeResult::Failed;
	//}
	//
	//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	//if (nullptr == NavSystem)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("No Creature in Navi"));
	//	return EBTNodeResult::Failed;
	//}
	//
	//return EBTNodeResult::InProgress;
}

void UBTTask_FindPlayerLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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

		FVector fTargetLastLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, fTargetLastLocation);
		
		// Move to last location of detected target.	
		UWorld* World = Reaper->GetWorld();
		if (nullptr == World)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 200.0f, fNextPatrol))
		{
			ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::PatrolPosKey, fNextPatrol.Location);

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


		FVector fTargetLastLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, fTargetLastLocation);

		// Move to last location of detected target.	
		UWorld* World = Runner->GetWorld();
		if (nullptr == World)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Get World Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Nav System Initialization Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 200.0f, fNextPatrol))
		{
			RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::PatrolPosKey, fNextPatrol.Location);
			
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


		FVector fTargetLastLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(BruteAI, fTargetLastLocation);
		
		// Move to last location of detected target.	
		UWorld* World = Brute->GetWorld();
		if (nullptr == World)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Get World Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Nav System Initialization Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 200.0f, fNextPatrol))
		{
			BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::PatrolPosKey, fNextPatrol.Location);
			
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	//ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	//AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	//APawn* Creature = nullptr;
	//AReaper_cpp* Reaper = nullptr;
	//ARunner_cpp* Runner = nullptr;
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
	//bool bCreature = false;
	//bool bReaper = false;
	//
	////if (nullptr == Creature)
	////{
	////	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Creature Initialization failed"));
	////	/*FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	////	return;*/
	////	bCreature = true;
	////}
	//
	//if (nullptr == Creature)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Reaper Initialization failed"));
	//	bReaper = true;
	//}
	//
	///*if (bCreature && bReaper)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	///}*/
	//
	//if (bReaper)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}
//
	//
	//FVector fTargetLastLocation(0.f, 0.f, 0.f);
	//
	//if(ReaperAI)
	//{
	//	fTargetLastLocation = ReaperAI->GetBlackboardComponent()->GetValueAsVector(ACreatureAI::TargetLocation);
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, fTargetLastLocation);
	//}
	//	
	//if(RunnerAI) 
	//{
	//	fTargetLastLocation = RunnerAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Runner::TargetLocation);
	//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, fTargetLastLocation);
	//}
	//// Move to last location of detected target.	
	//UWorld* World = Creature->GetWorld();
	////if (!bCreature)
	////{
	////	//Creature->EndChase();
	//
	////	World = Creature->GetWorld();
	////}
	////if (!bReaper)
	////{
	////	//Reaper->EndChase();
	////	World = Reaper->GetWorld();
	////}
	//
	//if (nullptr == World)
	//{
	////	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Get World Failed"));
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}
	//
	//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
	//if (nullptr == NavSystem)
	//{
	////	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Nav System Initialization Failed"));
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}
	//
	//FNavLocation fNextPatrol;
	//
	//if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 200.0f, fNextPatrol))
	//{
	//	if(ReaperAI)
	//		ReaperAI->GetBlackboardComponent()->SetValueAsVector(ACreatureAI::TargetLocation, fNextPatrol.Location);
	//
	//	if(RunnerAI)
	//		RunnerAI->GetBlackboardComponent()->SetValueAsVector(AAIController_Runner::TargetLocation, fNextPatrol.Location);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}
}