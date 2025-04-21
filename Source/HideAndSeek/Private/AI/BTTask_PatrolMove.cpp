// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_PatrolMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AISystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AI/AIController_Reaper.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Shadow.h"
#include "AI/YokaiAIController.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Shadow_cpp.h"
#include "Tasks/AITask_MoveTo.h"
#include "Player/HorrorGameCharacter.h"

UBTTask_PatrolMove::UBTTask_PatrolMove()
{
	NodeName = TEXT("PatrolMove");
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_PatrolMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AIController == nullptr) // AI 컴포넌트의 Controller가 없다면 실패를 리턴시킨다.
	{
		return EBTNodeResult::Failed; 
	}
	
	return EBTNodeResult::InProgress;
}

/*
여긴 리팩토링하려고 했으나, Brute의 특수 케이스 때문에 포기.
*/
void UBTTask_PatrolMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());

	// AI Controller가 있는 경우에만 아래의 동작을 수행한다.
	if (YokaiAI)
	{
		if (AAIController_Reaper* ReaperAI = Cast<AAIController_Reaper>(YokaiAI))
		{
			AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

			if (nullptr == Reaper)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = ReaperAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Reaper::PatrolPosKey);
			AActor* PatrolTarget= Cast<AActor>(ReaperAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Reaper::PatrolTargetKey));

			bool bCanSeePlayer = ReaperAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Reaper::CanSeePlayer);
			bool bNoiseDetected = ReaperAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Reaper::NoiseDetected);
			float Distance = 200.f;
			FVector ReaperLocation = Reaper->GetActorLocation(); // 리퍼의 Z 값은 132(132.400002)

			// 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우, 
			if (bCanSeePlayer)
			{
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
				ReaperAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				ReaperLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, ReaperLocation));
			}
			// 플레이어를 추격 중이 아닌 상황일 경우
			else
			{
				// 타이머나 알람이 울리는 소리를 들었을 경우
				if (bNoiseDetected)
				{
					FVector TargetLocation = ReaperAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Reaper::TargetLocation); // 타겟 위치
					AActor* NoiseTarget = Cast<AActor>(ReaperAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Reaper::NoiseTargetKey)); // 타겟

					// 소리 근원지 타겟 존재 시
					if (NoiseTarget)
					{
						ReaperAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ReaperLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), ReaperLocation));
					}
					else
					{
						// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
						ReaperAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ReaperLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, ReaperLocation));
					}
				}
				// 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				else
				{
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
					ReaperAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					ReaperLocation.Z -= 122.400002f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), ReaperLocation));
				}
			}
			
			// 리퍼와 패트롤 포인트의 거리 차이는 90.8으로 나옴

			if (Distance <= 100.f)
			{
				Reaper->SetPatrolSuccess(true);
			}
		
			if (Reaper->GetPatrolSuccess())
			{
				Reaper->SetPatrolSuccess(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				UE_LOG(LogTemp, Warning, TEXT("End Task"));
				return;
			}
		}

		else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(YokaiAI))
		{
			ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

			if (nullptr == Runner)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = RunnerAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Runner::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(RunnerAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Runner::PatrolTargetKey));
			
			bool bCanSeePlayer = RunnerAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
			bool bNoiseDetected = RunnerAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Runner::NoiseDetected);

			FVector RunnerLocation = Runner->GetActorLocation(); // 러너의 Z 값은 100(100.15)
			
			float Distance = 200.f;

			// 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			if (bCanSeePlayer)
			{
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
				RunnerAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				RunnerLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, RunnerLocation));
			}
			// 플레이어를 추격 중이 아닌 상황일 경우
			else
			{
				// 타이머나 알람이 울리는 소리를 들었을 때
				if (bNoiseDetected)
				{
					FVector TargetLocation = RunnerAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Runner::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(RunnerAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Runner::NoiseTargetKey));
					
					// 타겟이 존재할 땐 MoveToActor
					if (NoiseTarget)
					{
						RunnerAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						RunnerLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), RunnerLocation));
					}
					// 존재 안할 땐, MoveToLocation
					else
					{
						// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
						RunnerAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						RunnerLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, RunnerLocation));
					}
				}
				// 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				else
				{
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
					RunnerAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					RunnerLocation.Z -= 90.15f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), RunnerLocation));
				}
			}
			
			// 러너와 패트롤 포인트의 거리 차이는 80으로 나옴

			if (Distance <= 80.f)
			{
				Runner->SetPatrolSuccess(true);
			}

			if (Runner->GetPatrolSuccess())
			{
				Runner->SetPatrolSuccess(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				UE_LOG(LogTemp, Warning, TEXT("End Task"));
				return;
			}
		}

		else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(YokaiAI))
		{
			ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

			if (nullptr == Brute)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = BruteAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Brute::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(BruteAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Brute::PatrolTargetKey));
			
			bool bCanSeePlayer = BruteAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
			bool bNoiseDetected = BruteAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::NoiseDetected);

			FVector BruteLocation = Brute->GetActorLocation(); // 브루트의 Z 값은 100
			
			float Distance = 200.f;

			// 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			if (bCanSeePlayer)
			{
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
				BruteAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				BruteLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, BruteLocation));
			}
			// 플레이어를 추격 중이 아닌 상황일 경우
			else
			{
				// 플레이어나 무언가가 내는 소리를 들었을 경우
				if (bNoiseDetected)
				{
					FVector TargetLocation = BruteAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Brute::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(BruteAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Brute::NoiseTargetKey));
					
					// 타겟이 존재하면 MoveToActor로
					if (NoiseTarget)
					{
						BruteAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						BruteLocation.Z = NoiseTarget->GetActorLocation().Z;
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), BruteLocation));
					}
					else
					{
						// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
						BruteAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						BruteLocation.Z = TargetLocation.Z;
						Distance = FMath::Abs(FVector::Distance(TargetLocation, BruteLocation));
					}
				}
				else
				{
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
					BruteAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					BruteLocation.Z -= 90.f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), BruteLocation));
				}
			}
			
			// 브루트와 패트롤 포인트의 거리 차이는 76으로 나옴
			if (Distance <= 80.f)
			{
				Brute->SetPatrolSuccess(true);
			}

			if (Brute->GetPatrolSuccess())
			{
				Brute->SetPatrolSuccess(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				UE_LOG(LogTemp, Warning, TEXT("End Task"));
				return;
			}
		}

		else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(YokaiAI))
		{
			AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

			if (nullptr == Shadow)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = ShadowAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Shadow::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(ShadowAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Shadow::PatrolTargetKey));

			bool bCanSeePlayer = ShadowAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Shadow::CanSeePlayer);
			bool bNoiseDetected = ShadowAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Shadow::NoiseDetected);

			FVector ShadowLocation = Shadow->GetActorLocation(); // 러너의 Z 값은 100(100.15)

			float Distance = 200.f;

			// 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			if (bCanSeePlayer)
			{
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
				ShadowAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				ShadowLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, ShadowLocation));
			}
			// 플레이어를 추격 중이 아닌 상황일 경우
			else
			{
				// 타이머나 알람이 울리는 소리를 들었을 때
				if (bNoiseDetected)
				{
					FVector TargetLocation = ShadowAI->GetBlackboardComponent()->GetValueAsVector(AAIController_Shadow::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(ShadowAI->GetBlackboardComponent()->GetValueAsObject(AAIController_Shadow::NoiseTargetKey));

					// 타겟이 존재할 땐 MoveToActor
					if (NoiseTarget)
					{
						ShadowAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ShadowLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), ShadowLocation));
					}
					// 존재 안할 땐, MoveToLocation
					else
					{
						// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
						ShadowAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ShadowLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, ShadowLocation));
					}
				}
				// 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				else
				{
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
					ShadowAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					ShadowLocation.Z -= 90.15f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), ShadowLocation));
				}
			}

			// 러너와 패트롤 포인트의 거리 차이는 80으로 나옴

			if (Distance <= 80.f)
			{
				Shadow->SetPatrolSuccess(true);
			}

			if (Shadow->GetPatrolSuccess())
			{
				Shadow->SetPatrolSuccess(false);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				UE_LOG(LogTemp, Warning, TEXT("End Task"));
				return;
			}
			}
	}
	// AI Controller가 없는 경우엔, 그냥 실패했다고 알리고 Task를 종료한다.
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}