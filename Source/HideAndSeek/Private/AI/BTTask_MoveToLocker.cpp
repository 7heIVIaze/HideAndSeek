// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_MoveToLocker.h"
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

	if (AIController == nullptr) // AI 컴포넌트의 Controller가 없다면 실패를 리턴시킨다.
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress; // 동작 수행은 Tick Task를 통해 관리하므로 InProgress를 리턴시켜 아직 수행 중임을 알림.
}

void UBTTask_MoveToLocker::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController) // AI Controller가 있는 경우에만 아래의 동작 수행
	{
		// 그 컨트롤러가 Reaper의 컨트롤러라면
		if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
		{
			AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

			if (nullptr == Reaper)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 블랙보드에 저장된 Locker Target을 가져옴
			AActor* Target = Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::LockerTargetKey));

			if (nullptr == Target) // 타겟이 없으면 Fail 리턴시키고 task 종료시킴
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 해당 target으로 이동시킴
			ReaperAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			// 공격 모션이 끝난 이후 Locker Target과 Locker Lighting을 초기화함.
			if (Reaper->GetAnimFinish())
			{
				ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, nullptr);
				ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
		// 그 컨트롤러가 Runner의 컨트롤러라면
		else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
		{
			ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

			if (nullptr == Runner)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 블랙보드에 저장된 Locker Target을 가져옴
			AActor* Target = Cast<AActor>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::LockerTargetKey));

			if (nullptr == Target)
			{
				UE_LOG(LogTemp, Warning, TEXT("MoveToLocker ERROR: Target Not Found"));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 해당 target으로 이동시킴
			UE_LOG(LogTemp, Warning, TEXT("MoveToLocker Log: Move to Target"));
			RunnerAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			// 공격 모션이 끝난 이후 Locker Target과 Locker Lighting을 초기화함.
			if (Runner->GetAnimFinish())
			{
				RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, nullptr);
				RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
		// 그 컨트롤러가 Brute의 컨트롤러라면
		else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
		{
			ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

			if (nullptr == Brute)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 블랙보드에 저장된 Locker Target을 가져옴
			AActor* Target = Cast<AActor>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::LockerTargetKey));

			if (nullptr == Target)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 해당 target으로 이동시킴
			BruteAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			// 공격 모션이 끝난 이후 Locker Target과 Locker Lighting을 초기화함.
			if (Brute->GetAnimFinish())
			{
				BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, nullptr);
				BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
		// 그 컨트롤러가 Shadow의 컨트롤러라면
		else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
		{
			AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

			if (nullptr == Shadow)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 블랙보드에 저장된 Locker Target을 가져옴
			AActor* Target = Cast<AActor>(ShadowAI->GetBlackboard()->GetValueAsObject(AAIController_Shadow::LockerTargetKey));

			if (nullptr == Target)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			// 해당 target으로 이동시킴
			ShadowAI->MoveToActor(Target, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			// 공격 모션이 끝난 이후 Locker Target과 Locker Lighting을 초기화함.
			if (Shadow->GetAnimFinish())
			{
				ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, nullptr);
				ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}
	}
	else // AI Controller가 없으면 Fail을 리턴
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}