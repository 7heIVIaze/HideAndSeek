// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_FindPlayerLocation.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Player/HorrorGameCharacter.h"

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
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	// 이 아래는 해당 컨트롤러가 소유한 Pawn을 가져오고 월드에 배치된 Nav Mesh를 가져옴.
	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		if (nullptr == Yokai)
		{
			return EBTNodeResult::Failed;
		}

		UWorld* World = Yokai->GetWorld();

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
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	// 그 컨트롤러가 valid한 경우
	if (YokaiAI)
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());

		if (nullptr == Yokai)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		// 블랙보드에 있는 Target Location을 가져와 해당 위치로 이동시킴. 이 때, 이동 중 추격이 뜰 수 있기 때문에 중간에 멈출 수 있도록 SimpleMove로 구현함.
		FVector fTargetLastLocation = YokaiAI->GetBlackboardComponent()->GetValueAsVector(AYokaiAIController::TargetLocation);
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(YokaiAI, fTargetLastLocation);

		// Move to last location of detected target.	
		UWorld* World = Yokai->GetWorld();
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
			YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::PatrolPosKey, fNextPatrol.Location);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
}