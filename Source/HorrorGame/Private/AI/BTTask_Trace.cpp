// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTTask_Trace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"


UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("Trace");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Trace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

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

	/*ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	APawn* Creature = nullptr;
	AReaper_cpp* Reaper = nullptr;
	ARunner_cpp* Runner = nullptr;

	if (nullptr == Creature)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creature Initialize Failed"));
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Creature in Nav"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;*/
	return EBTNodeResult::Failed;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

		FVector TargetLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation);

		if (nullptr == pTarget) // if no target
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, TargetLocation);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		auto PlayerController = Cast<APlayerController>(pTarget->GetController());

		// move to actor
		UAIBlueprintHelperLibrary::SimpleMoveToActor(ReaperAI, pTarget);

		// if distance is under than 150, stop and set task succeed
		if (Reaper->GetAnimFinish())
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reaper Animation Finish")));
			/*if (pTarget->GetIsHiding())
				pTarget->SetPlayerStatus(Player_Status::Survive);
			else
				pTarget->SetPlayerStatus(Player_Status::Died);*/
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

		FVector TargetLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);

		if (nullptr == pTarget) // if no target
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, TargetLocation);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		auto PlayerController = Cast<APlayerController>(pTarget->GetController());

		// move to actor
		UAIBlueprintHelperLibrary::SimpleMoveToActor(RunnerAI, pTarget);

		if (Runner->GetAnimFinish())
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Runner Animation Finish")));
			/*if (pTarget->GetIsHiding())
				pTarget->SetPlayerStatus(Player_Status::Survive);
			else
				pTarget->SetPlayerStatus(Player_Status::Died);*/
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}

		AHorrorGameCharacter* pTarget = nullptr;

		pTarget = Cast<AHorrorGameCharacter>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::TargetKey));

		FVector TargetLocation = BruteAI->GetBlackboard()->GetValueAsVector(AAIController_Brute::TargetLocation);

		if (nullptr == pTarget) // if no target
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(BruteAI, TargetLocation);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		auto PlayerController = Cast<APlayerController>(pTarget->GetController());

		// move to actor
		UAIBlueprintHelperLibrary::SimpleMoveToActor(BruteAI, pTarget);

		if (Brute->GetAnimFinish())
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Brute Animation Finish")));
			
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}

	//ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	//AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	//APawn* Creature = nullptr;
	//AReaper_cpp* Reaper = nullptr;
	//ARunner_cpp* Runner = nullptr;

	//if (ReaperAI)
	//{
	//	Creature = ReaperAI->GetPawn();
	//	Reaper = Cast<AReaper_cpp>(Creature);
	//}

	//if (RunnerAI)
	//{
	//	Creature = RunnerAI->GetPawn();
	//	Runner = Cast<ARunner_cpp>(Creature);
	//}

	//if (nullptr == Creature)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//	return;
	//}

	//AHorrorGameCharacter* pTarget = nullptr;

	//if(ReaperAI)
	//	pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

	//if(RunnerAI)
	//	pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));
	//

	//FVector TargetLocation(0.f, 0.f, 0.f);
	//
	//if(ReaperAI)
	//	TargetLocation = ReaperAI->GetBlackboard()->GetValueAsVector(ACreatureAI::TargetLocation);
	//
	//if(RunnerAI)
	//	TargetLocation = RunnerAI->GetBlackboard()->GetValueAsVector(AAIController_Runner::TargetLocation);

	//auto PlayerController = Cast<APlayerController>(pTarget->GetController());
	//

	//if (nullptr == pTarget) // if no target
	//{
	//	// Controller->StopMovement(); // 왜 여기서 에러가 뜨는 걸까
	//	/*if(!bCreature)
	//		Creature->EndChase();
	//	if(!bReaper)
	//		Reaper->EndChase();*/
	//	if(ReaperAI)
	//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ReaperAI, TargetLocation);

	//	if(RunnerAI)
	//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(RunnerAI, TargetLocation);

	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	return;
	//}

	//// move to actor
	//if(ReaperAI)
	//	UAIBlueprintHelperLibrary::SimpleMoveToActor(ReaperAI, pTarget);

	//if(RunnerAI)
	//	UAIBlueprintHelperLibrary::SimpleMoveToActor(RunnerAI, pTarget);
	////if(!bCreature) 
	////	Creature->StartChase();
	////if(!bReaper)
	////	Reaper->StartChase();

	//// Caculate distance between creature and player
	///*FVector vCreatureLoc(0.f, 0.f, 0.f);
	//if(!bCreature)
	//	vCreatureLoc = Creature->GetActorLocation();
	//if (!bReaper)
	//	vCreatureLoc = Reaper->GetActorLocation();
	//FVector vTargetLoc = pTarget->GetActorLocation();
	//vCreatureLoc.Z = 0.f;
	//vTargetLoc.Z = 0.f;*/

	//// float fDistance = FVector::Distance(vCreatureLoc, vTargetLoc);

	//// UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), fDistance);
	//// if distance is under than 150, stop and set task succeed
	//if (Reaper)
	//{
	//	if (Reaper->GetAnimFinish())
	//		{ 
	//			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//			if (pTarget->GetIsHiding())
	//				pTarget->SetPlayerStatus(Player_Status::Survive);
	//			else
	//				pTarget->SetPlayerStatus(Player_Status::Died);
	//			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
	//			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
	//			return;
	//		}
	//}

	//if (Runner)
	//{
	//	if (Reaper->GetAnimFinish())
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//		if (pTarget->GetIsHiding())
	//			pTarget->SetPlayerStatus(Player_Status::Survive);
	//		else
	//			pTarget->SetPlayerStatus(Player_Status::Died);
	//		RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
	//		RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
	//		return;
	//	}
	//}
	//
	///*if (fDistance <= 100.0f)
	//{
	//	Controller->StopMovement();*/

	//	// FVector vDir = Reaper->GetActorLocation() - pTarget->GetActorLocation();
	//	// vDir.Normalize();
	//	// pTarget->SetActorRotation(FRotator(0.f, vDir.Rotation().Yaw, 0.f));
	//	// Reaper->SetIsCatch(true);

	//	//if (Reaper->GetIsCatch() && pTarget->GetPlayerStatus() == Player_Status::Survive && !pTarget->GetIsHiding())
	//	//{
	//	//	pTarget->SetPlayerStatus(Player_Status::Catched);
	//	//	/*FRotator CameraRotation = FRotationMatrix::MakeFromX(Reaper->GetActorLocation() - pTarget->GetActorLocation()).Rotator();
	//	//	
	//	//	pTarget->GetCameraComponent()->SetWorldRotation(CameraRotation);*/

	//	//	FVector CameraRot = Reaper->GetActorLocation() - pTarget->GetActorLocation();
	//	//	pTarget->AddControllerYawInput(CameraRot.X);
	//	//}
	//	
	////}

}