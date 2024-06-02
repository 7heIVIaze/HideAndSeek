// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_Trace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Rampage.h"
#include "AI/AIController_Shadow.h"
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
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creature Initialize Failed"));
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Rampage* RampageAI = Cast<AAIController_Rampage>(AIController))
	{
		ARampage_cpp* Rampage = Cast<ARampage_cpp>(RampageAI->GetPawn());

		if (nullptr == Rampage)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creature Initialize Failed"));
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
	{
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

		if (nullptr == Shadow)
		{
			UE_LOG(LogTemp, Warning, TEXT("Creature Initialize Failed"));
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
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 그 컨트롤러가 Reaper의 컨트롤러면
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

		FVector TargetLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, TargetLocation);
			ReaperAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// Target이 있으면 Target을 향해 이동함
		// move to actor
		// UAIBlueprintHelperLibrary::SimpleMoveToActor(ReaperAI, pTarget);
		ReaperAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Reaper->GetAnimFinish())
		{
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	// 컨트롤러가 Runner의 컨트롤러면
	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

		FVector TargetLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			// UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, TargetLocation);
			RunnerAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// Target이 있으면 Target을 향해 이동함
		// move to actor
		//UAIBlueprintHelperLibrary::SimpleMoveToActor(RunnerAI, pTarget);
		RunnerAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Runner->GetAnimFinish())
		{
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	// 그 컨트롤러가 Brute의 컨트롤러면
	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::TargetKey));

		FVector TargetLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(BruteAI, TargetLocation);
			BruteAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// Target이 있으면 Target을 향해 이동함
		// move to actor
		// UAIBlueprintHelperLibrary::SimpleMoveToActor(BruteAI, pTarget);
		BruteAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Brute->GetAnimFinish())
		{
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Brute Animation Finish")));
			*/
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	// 그 컨트롤러가 Rampage의 컨트롤러면
	else if (AAIController_Rampage* RampageAI = Cast<AAIController_Rampage>(AIController))
	{
		ARampage_cpp* Rampage = Cast<ARampage_cpp>(RampageAI->GetPawn());

		if (nullptr == Rampage)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(RampageAI->GetBlackboard()->GetValueAsObject(AAIController_Rampage::TargetKey));

		FVector TargetLocation = RampageAI->GetBlackboard()->GetValueAsVector(AAIController_Rampage::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			//UAIBlueprintHelperLibrary::SimpleMoveToLocation(BruteAI, TargetLocation);
			RampageAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, RampageAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// Target이 있으면 Target을 향해 이동함
		// move to actor
		// UAIBlueprintHelperLibrary::SimpleMoveToActor(BruteAI, pTarget);
		RampageAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, RampageAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Rampage->GetAnimFinish())
		{
			RampageAI->GetBlackboard()->SetValueAsObject(AAIController_Rampage::TargetKey, nullptr);
			RampageAI->GetBlackboard()->SetValueAsBool(AAIController_Rampage::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	// 그 컨트롤러가 Shadow의 컨트롤러면
	else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
	{
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

		if (nullptr == Shadow)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		// 블랙보드로부터 Target과 Target Location을 가져옴
		pTarget = Cast<AHorrorGameCharacter>(ShadowAI->GetBlackboard()->GetValueAsObject(AAIController_Shadow::TargetKey));

		FVector TargetLocation = ShadowAI->GetBlackboard()->GetValueAsVector(AAIController_Shadow::TargetLocation);

		// Target이 없을 경우 TargetLocation을 향해 이동함
		if (nullptr == pTarget) // if no target
		{
			// UAIBlueprintHelperLibrary::SimpleMoveToLocation(ShadowAI, TargetLocation);
			ShadowAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
				bProjectGoalLocation, bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// Target이 있으면 Target을 향해 이동함
		// move to actor
		//UAIBlueprintHelperLibrary::SimpleMoveToActor(ShadowAI, pTarget);
		ShadowAI->MoveToActor(pTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
			bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);

		// 공격 애니메이션이 끝나면 Target과 CanSeePlayer를 초기화함.
		if (Shadow->GetAnimFinish())
		{
			ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::TargetKey, nullptr);
			ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}