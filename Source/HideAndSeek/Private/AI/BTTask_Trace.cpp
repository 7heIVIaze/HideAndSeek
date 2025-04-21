// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_Trace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Rampage_cpp.h"
#include "AI/Shadow_cpp.h"

// AI가 플레이어를 추격하는 로직
UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("Trace");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	// AI 컨트롤러가 존재할 경우 InProgress 상태로, 아니면 fail로 설정함.
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		// 컨트롤러가 가진 캐릭터도 없을 경우 fail로 설정.
		if (nullptr == Yokai)
		{
			UE_LOG(LogTemp, Warning, TEXT("Yokai Initialize Failed"));
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		if (nullptr == Yokai)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(YokaiAI->GetBlackboardComponent()->GetValueAsObject(AYokaiAIController::TargetKey));

		FVector TargetLocation = YokaiAI->GetBlackboardComponent()->GetValueAsVector(AYokaiAIController::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			YokaiAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, YokaiAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
		// Target이 있으면 Target을 향해 이동함
		// move to actor
		YokaiAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, YokaiAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Yokai->GetAnimFinish())
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, nullptr);
			YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}