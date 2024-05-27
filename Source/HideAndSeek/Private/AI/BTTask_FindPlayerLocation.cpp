// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_FindPlayerLocation.h"
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
#include "HideAndSeek/HorrorGameCharacter.h"

// AI가 플레이어를 추격하다 놓쳤을 경우/어그로에 끌렸을 경우에 그 주위를 맴도는 로직
UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation()
{
	NodeName = TEXT("FindPlayerLocation");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	// 이 Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 이 아래는 해당 컨트롤러가 소유한 Pawn을 가져오고 월드에 배치된 Nav Mesh를 가져옴.
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			return EBTNodeResult::Failed;
		}

		UWorld* World = Reaper->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
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

		UWorld* World = Runner->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
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
			return EBTNodeResult::Failed;
		}

		UWorld* World = Brute->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
	{
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

		if (nullptr == Shadow)
		{
			return EBTNodeResult::Failed;
		}

		UWorld* World = Shadow->GetWorld();

		if (nullptr == World)
		{
			return EBTNodeResult::Failed;
		}

		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
		if (nullptr == NavSystem)
		{
			return EBTNodeResult::Failed;
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTTask_FindPlayerLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 그 컨트롤러가 Reaper의 컨트롤러인 경우
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 블랙보드에 있는 Target Location을 가져와 해당 위치로 이동시킴. 이 때, 이동 중 추격이 뜰 수 있기 때문에 중간에 멈출 수 있도록 SimpleMove로 구현함.
		FVector fTargetLastLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, fTargetLastLocation);
		
		// Move to last location of detected target.	
		UWorld* World = Reaper->GetWorld();
		if (nullptr == World)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 월드에 배치된 Nav Mesh를 가져옴
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		// Target Location을 기준으로 150 이내의 랜덤 위치를 Patrol Pos로 저장시킴
		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 150.0f, fNextPatrol))
		{
			ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::PatrolPosKey, fNextPatrol.Location);

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

		// 블랙보드에 있는 Target Location을 가져와 해당 위치로 이동함. 이 때, 이둥 중에 추격 판정이 뜰 수도 있으므로 중간에 멈출 수 있도록 Simple Move로 구현.
		FVector fTargetLastLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, fTargetLastLocation);

		// Move to last location of detected target.	
		UWorld* World = Runner->GetWorld();
		if (nullptr == World)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 월드에 배치된 Nav Mesh를 가져옴.
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		// Target Location을 기준으로 150 이내의 랜덤 위치를 Patrol Pos로 저장시킴.
		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 150.0f, fNextPatrol))
		{
			RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::PatrolPosKey, fNextPatrol.Location);
			
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

		// 블랙보드에 있는 Target Location을 가져와 해당 위치로 이동함. 이 때, 이동 중 추격 판정이 뜰 수도 있으므로 중간에 멈출 수 있도록 Simple Move로 구현함.
		FVector fTargetLastLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(BruteAI, fTargetLastLocation);
		
		// Move to last location of detected target.	
		UWorld* World = Brute->GetWorld();
		if (nullptr == World)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 월드에 배치된 Nav Mesh를 가져옴
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		// Target Location을 기준으로 150 이내의 랜덤 위치를 Patrol Pos로 저장시킴.
		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 150.0f, fNextPatrol))
		{
			BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::PatrolPosKey, fNextPatrol.Location);
			
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

		// 블랙보드에 있는 Target Location을 가져와 해당 위치로 이동함. 이 때, 이동 중 추격 판정이 뜰 수도 있으므로 중간에 멈출 수 있도록 Simple Move로 구현함.
		FVector fTargetLastLocation = ShadowAI->GetBlackboard()->GetValueAsVector(AAIController_Shadow::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ShadowAI, fTargetLastLocation);

		// Move to last location of detected target.	
		UWorld* World = Shadow->GetWorld();
		if (nullptr == World)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Get World Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 월드에 배치된 Nav Mesh를 가져옴
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(World);
		if (nullptr == NavSystem)
		{
			//	UE_LOG(LogTemp, Warning, TEXT("Task_FindPlayerLocation: Nav System Initialization Failed"));
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		FNavLocation fNextPatrol;

		// Target Location을 기준으로 150 이내의 랜덤 위치를 Patrol Pos로 저장시킴.
		if (NavSystem->GetRandomPointInNavigableRadius(fTargetLastLocation, 150.0f, fNextPatrol))
		{
			ShadowAI->GetBlackboard()->SetValueAsVector(AAIController_Shadow::PatrolPosKey, fNextPatrol.Location);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}