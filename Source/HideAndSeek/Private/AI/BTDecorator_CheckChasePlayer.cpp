// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTDecorator_CheckChasePlayer.h"
#include "AI/YokaiAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// 생성자
UBTDecorator_CheckChasePlayer::UBTDecorator_CheckChasePlayer()
{
	NodeName = TEXT("PlayerChase");
}

// 조건을 계산하는 함수
bool UBTDecorator_CheckChasePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	bool bPlayerChase = false;

	if (AIController)
	{
		// 요괴 AI의 플레이어를 보았는지 가져와서 갱신시켜줌.
		if (const auto YokaiAI = Cast<AYokaiAIController>(AIController))
		{
			bPlayerChase = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::CanSeePlayer);
		}
	}

	return bPlayerChase;
}