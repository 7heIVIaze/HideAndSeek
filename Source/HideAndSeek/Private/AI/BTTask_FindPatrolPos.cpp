// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 다음 패트롤 위치를 감지하는 동작을 수행하는 코드

#include "AI/BTTask_FindPatrolPos.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
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
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());
	
	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		// 요괴 controller가 요괴를 Possess하지 못한 상태면(즉 Reaper가 없으면) Fail
		if (nullptr == Yokai)
		{
			return EBTNodeResult::Failed;
		}

		// PatronPoint1~25 중 하나를 다음 Patrol 위치로 가져오고 해당 위치를 Reaper Controller에 추가
		FVector NextLocation = Yokai->GetPatrolPoint();
		YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::PatrolPosKey, NextLocation);

		// 잘 들어갔으면 Succeed 리턴
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}