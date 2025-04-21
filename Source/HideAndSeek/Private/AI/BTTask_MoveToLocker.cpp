// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_MoveToLocker.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/HideObject.h"

// AI를 숨을 수 있는 오브젝트(옷장/캐비닛)로 이동시키는 로직
UBTTask_MoveToLocker::UBTTask_MoveToLocker()
{
	NodeName = TEXT("MoveToLocker");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveToLocker::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	// AI 컴포넌트의 Controller가 없다면 실패를 리턴시킨다.
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 동작 수행은 Tick Task를 통해 관리하므로 InProgress를 리턴시켜 아직 수행 중임을 알림.
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToLocker::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	// AI Controller가 있는 경우에만 아래의 동작 수행
	if (YokaiAI) 
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		if (nullptr == Yokai)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 블랙보드에 저장된 Locker Target을 가져옴
		AActor* Target = Cast<AActor>(YokaiAI->GetBlackboardComponent()->GetValueAsObject(AYokaiAIController::LockerTargetKey));

		// 타겟이 없으면 Fail 리턴시키고 task 종료시킴
		if (nullptr == Target)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 해당 target으로 이동시킴
		YokaiAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, YokaiAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 모션이 끝난 이후 Locker Target과 Locker Lighting을 초기화함.
		if (Yokai->GetAnimFinish())
		{
			YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::LockerTargetKey, nullptr);
			YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::LockerLighting, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	// AI Controller가 없으면 Fail을 리턴
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}