// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTDecorator_CheckStun.h"
#include "AI/YokaiAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTDecorator_CheckStun::UBTDecorator_CheckStun()
{
	NodeName = TEXT("IsStun");
}

// 조건을 판단하는 함수
bool UBTDecorator_CheckStun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	bool bStunned = false;

	if (AIController)
	{
		// 기절했는지 여부를 가져와서 갱신시켜줌.
		if (const auto YokaiAI = Cast<AYokaiAIController>(AIController))
		{
			bStunned = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::Stunned);
		}
	}

	return bStunned;
}