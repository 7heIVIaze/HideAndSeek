// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTService_Stuning.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
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
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());
	
	// AI에 있는 현재 상태를 가져옴
	bool bCurrentStatus = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::Stunned);
	bool bIsStunned = false;

	ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

	if (Yokai)
	{
		bIsStunned = Yokai->GetIsStunned();
	}

	if (bIsStunned != bCurrentStatus)
	{
		YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::Stunned, bIsStunned);
	}
}