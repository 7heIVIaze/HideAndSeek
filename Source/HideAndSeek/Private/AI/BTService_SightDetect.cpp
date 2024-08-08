// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTService_SightDetect.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "Player/HorrorGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_SightDetect::UBTService_SightDetect()
{
	NodeName = TEXT("SightDetect");
	FlashLightDist = 1600.f;
	CigarLightDist = 800.f;
	NoLightDist = 400.f;
	Interval = 0.5f;
}

void UBTService_SightDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	APawn* Creature = nullptr;

	/*if (nullptr == AIController)
	{
		return;
	}
	
	auto Creature = AIController->GetPawn();
	if (nullptr == Creature)
	{
		return;
	}

	UAIPerceptionComponent* AIPerceptionComp = AIController->GetPerceptionComponent();
	if (nullptr == AIPerceptionComp)
	{
		return;
	}

	TArray<AActor*> PerceivedActors;
	AIPerceptionComp->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	for (AActor* PerceivedActor : PerceivedActors)
	{
		AHorrorGameCharacter* perceivedPlayer = Cast<AHorrorGameCharacter>(PerceivedActor);
		if (perceivedPlayer)
		{
			float Dist = FVector::Distance(Creature->GetActorLocation(), perceivedPlayer->GetActorLocation());

			
			if (Dist <= NoLightDist)
			{
				AIController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, perceivedPlayer);
				AIController->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, perceivedPlayer->GetActorLocation());
			}

			else if (Dist <= CigarLightDist)
			{
				if (perceivedPlayer->bIsCigarLightOn)
				{
					AIController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, perceivedPlayer);
					AIController->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, perceivedPlayer->GetActorLocation());
				}
			}

			if (Dist <= FlashLightDist)
			{
				if (perceivedPlayer->bIsFlashLightOn)
				{
					AIController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, perceivedPlayer);
					AIController->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, perceivedPlayer->GetActorLocation());
				}
			}
		}
	}*/
	
	// 여기 아래까지는 이전 버전(AI Perception(PawnSensing))을 사용하지 않고 자체 콜리전을 사용하던 버전
	if (ReaperAI)
	{
		
		Creature = ReaperAI->GetPawn();
	}

	if (RunnerAI)
	{
		Creature = RunnerAI->GetPawn();
	}
	if (nullptr == Creature)
	{
		UE_LOG(LogTemp, Warning, TEXT("Service_SightDetect: Creature Init Failed"));
		return;
	}

	UWorld* world = Creature->GetWorld();
	if (nullptr == world)
	{
		UE_LOG(LogTemp, Warning, TEXT("Service_SightDetect: World Init Failed"));
		return;
	}

	// 콜리전 감지를 위한 기본 변수들
	FVector StartLocation = Creature->GetActorLocation();
	FVector ForwardVector = Creature->GetActorForwardVector();
	FVector EndLocation = StartLocation + ForwardVector * 1600.0f;
	FVector Extent(40.0f, 40.0f, 1.0f); // X, Y, Z 축 범위
	FHitResult HitResult;
	FCollisionQueryParams params(NAME_None, false, Creature);


	// 초기 버전 OverlapMultiByChannel로 물체 뒤 겹친 캐릭터 가져오게
	//bool bResult = world->OverlapMultiByChannel(
	//	OverlapResults,
	//	Creature->GetActorLocation(),
	//	qRotationMat,
	//	ECollisionChannel::ECC_GameTraceChannel6,
	//	//FCollisionShape::MakeCapsule(40.f, 1600.f),
	//	FCollisionShape::MakeBox(vTraceVec),
	//	params
	//);

	// 하나만 감지하게 SweepSingleByChannel로 변경했음
	bool bResult = world->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeBox(ForwardVector * 1600.f),
		params
	);

	//FColor DrawColor = bResult ? FColor::Red : FColor::Green;
	// float fHalfHeight = 1600.0f * 0.5f + 40.0f;
	//DrawDebugCapsule(world, vCenter, fHalfHeight, 50.0f, qRotationMat, DrawColor, false, 0.2f);

	// 플레이어 감지 시 로직
	if (bResult)
	{
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(HitResult.GetActor());
		if (pTarget && pTarget->GetController()->IsPlayerController() && !pTarget->GetIsHiding())
		{
			if (ReaperAI)
			{
				ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, pTarget);
				ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, pTarget->GetActorLocation());
			}

			if (RunnerAI)
			{
				RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, pTarget);
				RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, pTarget->GetActorLocation());
			}

			// DrawDebugCapsule(world, vCenter, fHalfHeight, 50.0f, qRotationMat, FColor::Red, false, 0.2f);
			DrawDebugBox(world, StartLocation, ForwardVector * 1600.f, FQuat::Identity, FColor::Red, false, 0.2f);
			DrawDebugLine(world, Creature->GetActorLocation(), pTarget->GetActorLocation(), FColor::Blue, false, 0.2f);
		}
		else
		{
			if(ReaperAI)
				ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);

			if(RunnerAI)
				RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
			
		}
		//for (auto OverlapResult : OverlapResults)
		//{
		//	AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(OverlapResult.GetActor());
		//	if (pTarget && pTarget->GetController()->IsPlayerController() && !pTarget->GetIsHiding())
		//	{
		//		AIController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, pTarget);
		//		AIController->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, pTarget->GetActorLocation());
		//		//AIController->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, true);

		//		DrawDebugCapsule(world, Center, 380.f, 20.f, qRotationMat, FColor::Red, false, 0.2f);

		//		DrawDebugPoint(world, pTarget->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
		//		DrawDebugLine(world, Creature->GetActorLocation(), pTarget->GetActorLocation(), FColor::Blue, false, 0.2f);
		//		return;
		//	}
		//	else
		//	{
		//		AIController->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
		//	//	AIController->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
		//	}
		//}
	}
	else
	{
		if(ReaperAI)
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);

		if(RunnerAI)
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
	//	AIController->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
	}
//	DrawDebugCapsule(world, vCenter, 380.f, 20.f, qRotationMat, FColor::Green, false, 0.2f);
	// DrawDebugCapsule(world, vCenter, fHalfHeight, 50.0f, qRotationMat, FColor::Green, false, 0.2f);
	DrawDebugBox(world, StartLocation, ForwardVector * 1600.f, FQuat::Identity, FColor::Green, false, 0.2f);
}