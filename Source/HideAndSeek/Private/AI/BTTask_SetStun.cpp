// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_SetStun.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"

// AI를 행동 불능 상태로 만드는 로직
UBTTask_SetStun::UBTTask_SetStun()
{
	NodeName = TEXT("SetStun");
}

EBTNodeResult::Type UBTTask_SetStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		if (nullptr == Yokai)
		{
			return EBTNodeResult::Failed;
		}

		// 요괴가 행동 불능 상태라면 컨트롤러에도 업데이트하고 Target을 초기화함
		bool stunned = Yokai->GetIsStunned();

		YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::Stunned, stunned);
		if (stunned)
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, nullptr);
		}

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}