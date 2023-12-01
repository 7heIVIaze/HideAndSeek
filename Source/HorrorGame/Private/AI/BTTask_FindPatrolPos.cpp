// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com
// 다음 패트롤 위치를 감지하는 동작을 수행하는 코드

#include "AI/BTTask_FindPatrolPos.h"
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

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPosTask");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIController = OwnerComp.GetAIOwner(); // Behavior Tree를 실행시키는 Component의 AI Controller

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController)) // AI Controller가 Reaper Controller일 경우 
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper) // Reaper Controller가 Reaper를 Possess하지 못한 상태면(즉 Reaper가 없으면) Fail
		{
			return EBTNodeResult::Failed;
		}

		//UWorld* World = Reaper->GetWorld();
		//
		//if (nullptr == World) // Reaper가 World에 속해있지 않으면 Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);

		//// if cannot find Nav mesh return false
		//if (nullptr == NavSystem) // 현재 Level에 깔려있는 Nav Mesh가 없으면 Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		//FVector const Origin = Reaper->GetActorLocation(); // 현재 Reaper의 위치
		//FNavLocation NextPatrol;

		FVector NextLocation = Reaper->GetPatrolPoint(); // 다음 Patrol 위치를 가져옴(PatrolPoint 1~25 중 하나) 
		ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::PatrolPosKey, NextLocation); // 해당 위치를 Reaper AI에 추가

		// FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded; // 잘 들어갔으면 Succeed 리턴
	}

	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController)) // AI Controller가 Runner Controller라면
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner) // Runner Controller가 Runner를 Possess하지 못했으면(즉 Runner가 없다면) Fail
		{
			return EBTNodeResult::Failed;
		}

		//UWorld* World = Runner->GetWorld();
		//
		//if (nullptr == World) // 현재 World에 Runner가 존재하지 않으면 Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		//// if cannot find Nav mesh return false
		//if (nullptr == NavSystem) // 현재 Level에 Nav Mesh가 깔려있지 않으면 Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		/*FVector const Origin = Runner->GetActorLocation();
		FNavLocation NextPatrol;*/

		FVector const NextLocation = Runner->GetPatrolPoint(); // 다음 Patrol 위치를 가져옴 (PatronPoint1~25 중 하나)
		RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::PatrolPosKey, NextLocation); // 해당 위치를 Runner Controller에 추가

		return EBTNodeResult::Succeeded; // Succeed 리턴
	}

	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController)) // AI Controller가 Brute Controller라면
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute) // 만약 Brute Controller가 Brute를 Possess하지 못한 상태라면(즉 Brute가 없다면) Fail
		{
			return EBTNodeResult::Failed;
		}

		//UWorld* World = Brute->GetWorld();

		//if (nullptr == World) // Brute로부터 World 오브젝트를 가져오지 못했다면(즉 World에 Brute가 없다면) Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		//UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		//// if cannot find Nav mesh return false
		//if (nullptr == NavSystem) // 현재 Level에 Nav Mesh가 깔려 있지 않은 상태라면 Fail
		//{
		//	return EBTNodeResult::Failed;
		//}

		/*FVector const Origin = Brute->GetActorLocation();
		FNavLocation NextPatrol;*/
		
		FVector NextLocation = Brute->GetPatrolPoint(); // 다음 Patrol 위치를 가져옴 (PatrolPoint1~25 중 하나)
		BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::PatrolPosKey, NextLocation); // 해당 위치를 Brute Controller에 추가

		return EBTNodeResult::Succeeded; // Succeed 반환
	}

	return EBTNodeResult::Failed;
//	ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
//	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
//	APawn* Creature = nullptr;
//	AReaper_cpp* Reaper = nullptr;
//	ARunner_cpp* Runner = nullptr;
//
//	if (ReaperAI)
//	{
//		Creature = ReaperAI->GetPawn();
//		Reaper = Cast<AReaper_cpp>(Creature);
//	}
//
//	if (RunnerAI)
//	{
//		Creature = RunnerAI->GetPawn();
//		Runner = Cast<ARunner_cpp>(Creature);
//	}
//
//	if (nullptr == Creature)
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("Task_FindPatrolPos: Creature Initialize Failed!"));
//		return EBTNodeResult::Failed;
//	}
//	
//	// UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Creature->GetWorld());
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
//	// if cannot find Nav mesh return false
//	if (nullptr == NavSystem)
//	{
//		//UE_LOG(LogTemp, Warning, TEXT("Task_FindPatrolPos: No Creature in Navi"));
//		return EBTNodeResult::Failed;
//	}
//
//	/*FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ACreatureAI::HomePosKey);
//	FNavLocation NextPatrol;*/	
//	FVector const Origin = Creature->GetActorLocation();
//	FNavLocation NextPatrol;
//	if (Reaper)
//	{
//		FVector NextLocation = Reaper->GetPatrolPoint();
//		ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, NextLocation);
//	}
//
//	if (Runner)
//	{
//		FVector NextLocation = Runner->GetPatrolPoint();
//		RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, NextLocation);
//	}
//
//	//if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, NextPatrol, nullptr))
//	//{
//	//	Controller->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, NextPatrol.Location);
//	////	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ACreatureAI::PatrolPosKey, NextPatrol.Location);
//	////	return EBTNodeResult::Succeeded;
//	//}
//	//UE_LOG(LogTemp, Warning, TEXT("Task_FindPatrolPos: Next Location: %s"), *NextPatrol.Location.ToString());
//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
//	return EBTNodeResult::Succeeded;
////	return EBTNodeResult::Failed;
}