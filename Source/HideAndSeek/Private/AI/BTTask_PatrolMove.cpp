// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_PatrolMove.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AISystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Shadow.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Shadow_cpp.h"
#include "Tasks/AITask_MoveTo.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Door_cpp.h"
#include "ClassroomDoorActor_cpp.h"
#include "MetalDoor_cpp.h"
#include "DoorInterface_cpp.h"

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

void UBTTask_PatrolMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController) // AI Controller가 있는 경우에만 아래의 동작을 수행한다.
	{
		if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
		{
			AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

			if (nullptr == Reaper)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::PatrolPosKey);
			AActor* PatrolTarget= Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::PatrolTargetKey));

			bool bCanSeePlayer = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::CanSeePlayer);
			bool bNoiseDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);
			float Distance = 200.f;
			FVector ReaperLocation = Reaper->GetActorLocation(); // 리퍼의 Z 값은 132(132.400002)

			/*if (DoorActor)
			{
				if (DoorActor->bIsDoorClosed && IsFacingDoor(ReaperAI, DoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문을 열도록 처리
				{
					ReaperAI->StopMovement(); // 이동을 멈춤

					DoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (ClassroomDoorActor)
			{
				if (ClassroomDoorActor->bIsDoorClosed && IsFacingDoor(ReaperAI, ClassroomDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					ReaperAI->StopMovement(); // 이동을 멈춤

					ClassroomDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (MetalDoorActor)
			{
				if (MetalDoorActor->bIsDoorClosed && IsFacingDoor(ReaperAI, MetalDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					ReaperAI->StopMovement(); // 이동을 멈춤

					MetalDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}*/

			if (bCanSeePlayer) // 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우, 
			{
				ReaperAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				ReaperLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, ReaperLocation));
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
			}
			else // 플레이어를 추격 중이 아닌 상황일 경우
			{
				if (bNoiseDetected) // 타이머나 알람이 울리는 소리를 들었을 경우
				{
					FVector TargetLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation); // 타겟 위치
					AActor* NoiseTarget = Cast<AActor>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::NoiseTargetKey)); // 타겟

					if (NoiseTarget) // 소리 근원지 타겟 존재 시
					{
						ReaperAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ReaperLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), ReaperLocation));
					}
					else
					{
						ReaperAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ReaperLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, ReaperLocation));
					}
					// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
				}
				else // 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				{
					ReaperAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, ReaperAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					ReaperLocation.Z -= 122.400002f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), ReaperLocation));
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
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

		else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
		{
			ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

			if (nullptr == Runner)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::PatrolTargetKey));
			
			bool bCanSeePlayer = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
			bool bNoiseDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected);

			FVector RunnerLocation = Runner->GetActorLocation(); // 러너의 Z 값은 100(100.15)
			
			float Distance = 200.f;

			/*if (DoorActor)
			{
				if (DoorActor->bIsDoorClosed && IsFacingDoor(RunnerAI, DoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문을 열도록 처리
				{
					RunnerAI->StopMovement(); // 이동을 멈춤

					DoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (ClassroomDoorActor)
			{
				if (ClassroomDoorActor->bIsDoorClosed && IsFacingDoor(RunnerAI, ClassroomDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					RunnerAI->StopMovement(); // 이동을 멈춤

					ClassroomDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (MetalDoorActor)
			{
				if (MetalDoorActor->bIsDoorClosed && IsFacingDoor(RunnerAI, MetalDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					RunnerAI->StopMovement(); // 이동을 멈춤

					MetalDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}*/

			if (bCanSeePlayer) // 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			{
				RunnerAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				RunnerLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, RunnerLocation));
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
			}
			else // 플레이어를 추격 중이 아닌 상황일 경우
			{
				if (bNoiseDetected) // 타이머나 알람이 울리는 소리를 들었을 때
				{
					FVector TargetLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::NoiseTargetKey));
					
					if (NoiseTarget) // 타겟이 존재할 땐 MoveToActor
					{
						RunnerAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						RunnerLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), RunnerLocation));
					}
					else // 존재 안할 땐, MoveToLocation
					{
						RunnerAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						RunnerLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, RunnerLocation));
					}
					// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
				}
				else // 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				{
					RunnerAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					RunnerLocation.Z -= 90.15f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), RunnerLocation));
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
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

		else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
		{
			ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

			if (nullptr == Brute)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::PatrolTargetKey));
			
			bool bCanSeePlayer = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
			bool bNoiseDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::NoiseDetected);

			FVector BruteLocation = Brute->GetActorLocation(); // 브루트의 Z 값은 100
			
			float Distance = 200.f;

			/*if (DoorActor)
			{
				if (DoorActor->bIsDoorClosed && IsFacingDoor(BruteAI, DoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문을 열도록 처리
				{
					BruteAI->StopMovement(); // 이동을 멈춤

					DoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (ClassroomDoorActor)
			{
				if (ClassroomDoorActor->bIsDoorClosed && IsFacingDoor(BruteAI, ClassroomDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					BruteAI->StopMovement(); // 이동을 멈춤

					ClassroomDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}
			if (MetalDoorActor)
			{
				if (MetalDoorActor->bIsDoorClosed && IsFacingDoor(BruteAI, MetalDoorActor)) // 문이 닫혀있는 상태라면, AI를 멈추고 문이 열도록 처리
				{
					BruteAI->StopMovement(); // 이동을 멈춤

					MetalDoorActor->AIInteract(); // 문을 열도록 처리

					GetWorld()->GetTimerManager().SetTimer(TimerHandle_WaitForDoor, FTimerDelegate::CreateLambda([&]()
					{
						// 내가 원하는 코드 구현

						// 타이머 초기화
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle_WaitForDoor);
					}), 2.f, false); // 문이 완전히 열리기까지 대기
				}
			}*/

			if (bCanSeePlayer) // 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			{
				BruteAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				BruteLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, BruteLocation));
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
			}
			else // 플레이어를 추격 중이 아닌 상황일 경우
			{
				if (bNoiseDetected) // 플레이어나 무언가가 내는 소리를 들었을 경우
				{
					FVector TargetLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::NoiseTargetKey));
					
					if (NoiseTarget) // 타겟이 존재하면 MoveToActor로
					{
						BruteAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						BruteLocation.Z = NoiseTarget->GetActorLocation().Z;
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), BruteLocation));
					}
					else
					{
						BruteAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						BruteLocation.Z = TargetLocation.Z;
						Distance = FMath::Abs(FVector::Distance(TargetLocation, BruteLocation));
						// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
					}
				}
				else
				{
					BruteAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, BruteAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					BruteLocation.Z -= 90.f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), BruteLocation));
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
				}
				// 브루트와 패트롤 포인트의 거리 차이는 76으로 나옴
			}
			
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

		else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
		{
			AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

			if (nullptr == Shadow)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}

			FVector PatrolLocation = ShadowAI->GetBlackboard()->GetValueAsVector(AAIController_Shadow::PatrolPosKey);
			AActor* PatrolTarget = Cast<AActor>(ShadowAI->GetBlackboard()->GetValueAsObject(AAIController_Shadow::PatrolTargetKey));

			bool bCanSeePlayer = ShadowAI->GetBlackboard()->GetValueAsBool(AAIController_Shadow::CanSeePlayer);
			bool bNoiseDetected = ShadowAI->GetBlackboard()->GetValueAsBool(AAIController_Shadow::NoiseDetected);

			FVector ShadowLocation = Shadow->GetActorLocation(); // 러너의 Z 값은 100(100.15)

			float Distance = 200.f;

			if (bCanSeePlayer) // 만약 플레이어를 추격하다가 플레이어를 놓쳤을 경우거나
			{
				ShadowAI->MoveToLocation(PatrolLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
					bProjectGoalLocation, bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
				ShadowLocation.Z = PatrolLocation.Z;
				Distance = FMath::Abs(FVector::Distance(PatrolLocation, ShadowLocation));
				// 추격하다 놓친 상태에서는 FindPlayerLocation에서 랜덤 순찰 지점을 PatrolPosKey에 저장하므로 Location임
			}
			else // 플레이어를 추격 중이 아닌 상황일 경우
			{
				if (bNoiseDetected) // 타이머나 알람이 울리는 소리를 들었을 때
				{
					FVector TargetLocation = ShadowAI->GetBlackboard()->GetValueAsVector(AAIController_Shadow::TargetLocation);
					AActor* NoiseTarget = Cast<AActor>(ShadowAI->GetBlackboard()->GetValueAsObject(AAIController_Shadow::NoiseTargetKey));

					if (NoiseTarget) // 타겟이 존재할 땐 MoveToActor
					{
						ShadowAI->MoveToActor(NoiseTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bAllowStrafe, RunnerAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ShadowLocation.Z = NoiseTarget->GetActorLocation().Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(NoiseTarget->GetActorLocation(), ShadowLocation));
					}
					else // 존재 안할 땐, MoveToLocation
					{
						ShadowAI->MoveToLocation(TargetLocation, AcceptableRadius, bStopOverlap, bUsePathfinding,
							bProjectGoalLocation, bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
						ShadowLocation.Z = TargetLocation.Z; // 그냥 액터의 Z 위치 동일화시킴
						Distance = FMath::Abs(FVector::Distance(TargetLocation, ShadowLocation));
					}
					// 소리를 들은 상태에서는 NoiseDetect에서 근원지를 TargetLocationKey에 저장하므로 Location임
				}
				else // 그것이 아닌 상황은 진짜 PatrolPoint를 향해 가는 것
				{
					ShadowAI->MoveToActor(PatrolTarget, AcceptableRadius, bStopOverlap, bUsePathfinding,
						bAllowStrafe, ShadowAI->GetDefaultNavigationFilterClass(), bAllowPartialPath);
					ShadowLocation.Z -= 90.15f;
					Distance = FMath::Abs(FVector::Distance(PatrolTarget->GetActorLocation(), ShadowLocation));
					// PatrolPoint는 PatrolTarget에 저장하니까 MoveToActor 사용
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
	else // AI Controller가 없는 경우엔, 그냥 실패했다고 알리고 Task를 종료한다.
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}