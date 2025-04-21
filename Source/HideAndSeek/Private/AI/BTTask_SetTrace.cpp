// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_SetTrace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "AI/Brute_cpp.h"

// AI의 추격 상태를 설정하는 로직
UBTTask_SetTrace::UBTTask_SetTrace()
{
	NodeName = TEXT("SetTrace");
}

EBTNodeResult::Type UBTTask_SetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

		// 블랙보드에 저장된 Target을 가져옴
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(YokaiAI->GetBlackboardComponent()->GetValueAsObject(AYokaiAIController::TargetKey));

		// Target이 null이라면 CanSeePlayer를 초기화하고 추격을 멈춤
		if (nullptr == pTarget)
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::CanSeePlayer, false);
			Yokai->EndChase();
		}
		// 반대라면 CanSeePlayer를 on하고 추격을 시작함
		else
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::CanSeePlayer, true);
			Yokai->StartChase();
		}

		// 블랙보드에 저장된 Noise Detect를 가져옴
		bool bNoiseDetected = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::NoiseDetected);

		// true라면 Noise Detect를 false로 변경하고, 소리 감지 범위도 줄이도록 함.
		if (bNoiseDetected)
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, false);
			
			// Brute인 경우, 감지 범위도 줄임.
			if (ABrute_cpp* Brute = Cast<ABrute_cpp>(Yokai))
			{
				Brute->ChangeNoiseRange(false);
			}
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}