// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com
// 다음 패트롤 위치를 감지하는 동작을 수행하는 코드

#include "AI/BTTask_FindPatrolPos.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Shadow.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Shadow_cpp.h"
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
	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	// AI Controller가 Reaper Controller일 경우 
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController)) 
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper) // Reaper Controller가 Reaper를 Possess하지 못한 상태면(즉 Reaper가 없으면) Fail
		{
			return EBTNodeResult::Failed;
		}

		// PatronPoint1~25 중 하나를 다음 Patrol 위치로 가져오고 해당 위치를 Reaper Controller에 추가
		FVector NextLocation = Reaper->GetPatrolPoint();
		ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::PatrolPosKey, NextLocation);

		return EBTNodeResult::Succeeded; // 잘 들어갔으면 Succeed 리턴
	}
	// AI Controller가 Runner Controller라면
	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner) // Runner Controller가 Runner를 Possess하지 못했으면(즉 Runner가 없다면) Fail
		{
			return EBTNodeResult::Failed;
		}

		// PatronPoint1~25 중 하나를 다음 Patrol 위치로 가져오고 해당 위치를 Runner Controller에 추가
		FVector const NextLocation = Runner->GetPatrolPoint(); 
		RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::PatrolPosKey, NextLocation); 

		return EBTNodeResult::Succeeded; // Succeed 리턴
	}
	// AI Controller가 Brute Controller라면
	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController)) 
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute) // 만약 Brute Controller가 Brute를 Possess하지 못한 상태라면(즉 Brute가 없다면) Fail
		{
			return EBTNodeResult::Failed;
		}

		// PatronPoint1~25 중 하나를 다음 Patrol 위치로 가져오고 해당 위치를 Brute Controller에 추가		
		FVector NextLocation = Brute->GetPatrolPoint();
		BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::PatrolPosKey, NextLocation);

		return EBTNodeResult::Succeeded; // Succeed 반환
	}
	// AI Controller가 Runner Controller라면
	else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController)) 
	{
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

		if (nullptr == Shadow) // Runner Controller가 Runner를 Possess하지 못했으면(즉 Runner가 없다면) Fail
		{
			return EBTNodeResult::Failed;
		}

		// PatronPoint1~25 중 하나를 다음 Patrol 위치로 가져오고 해당 위치를 Shadow Controller에 추가
		FVector const NextLocation = Shadow->GetPatrolPoint();
		ShadowAI->GetBlackboard()->SetValueAsVector(AAIController_Shadow::PatrolPosKey, NextLocation);

		return EBTNodeResult::Succeeded; // Succeed 리턴
	}

	return EBTNodeResult::Failed;
}