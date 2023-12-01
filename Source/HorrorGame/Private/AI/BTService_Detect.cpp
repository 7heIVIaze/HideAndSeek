// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTService_Detect.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "HorrorGame/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();
	/*ACreatureAI* ReaperAI = Cast<ACreatureAI>(OwnerComp.GetAIOwner());
	AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(OwnerComp.GetAIOwner());
	APawn* Creature = nullptr;
	bool bDetected = false;*/

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());
		bool bDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::CanSeePlayer);
		if (nullptr == Reaper) return;

		UWorld* World = Reaper->GetWorld();

		FVector Center = Reaper->GetActorLocation();
		float DetectRadius = 400.f;
		if (bDetected)
			DetectRadius = 1000.f;
		//FVector DetectSize = FVector(100.0f, 800.0f, 10.0f);

		if (nullptr == World) return;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Reaper);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			//FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(),
			ECollisionChannel::ECC_GameTraceChannel3,
			//FCollisionShape::MakeCapsule(20.f, 400.f),
			FCollisionShape::MakeSphere(DetectRadius),
			//FCollisionShape::MakeBox(DetectSize),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

		if (bResult)
		{
			for (auto OverlapResult : OverlapResults)
			{
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, PlayerCharacter);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, PlayerCharacter->GetActorLocation());

						// Show radius
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						DrawDebugLine(World, Reaper->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller

				else if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Cabinet);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
						}
						else
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
						}
					}
				}
				else if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Wardrobe);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
						}
						else
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
						}
					}
				}
				else
				{
					// set null
					ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}

	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());
		bool bDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
		if (nullptr == Runner) return;

		UWorld* World = Runner->GetWorld();

		FVector Center = Runner->GetActorLocation();
		float DetectRadius = 400.f;
		if (bDetected)
			DetectRadius = 1600.f;
		//FVector DetectSize = FVector(100.0f, 800.0f, 10.0f);

		if (nullptr == World) return;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Runner);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			//FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(),
			ECollisionChannel::ECC_GameTraceChannel3,
			//FCollisionShape::MakeCapsule(20.f, 400.f),
			FCollisionShape::MakeSphere(DetectRadius),
			//FCollisionShape::MakeBox(DetectSize),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

		if (bResult)
		{
			for (auto OverlapResult : OverlapResults)
			{
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, PlayerCharacter);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, PlayerCharacter->GetActorLocation());
						// Show radius
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						DrawDebugLine(World, Runner->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller

				else if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, Cabinet);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
						}
						else
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
						}
					}
				}
				else if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, Wardrobe);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
						}
						else
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
						}
					}
				}
				else
				{
					// set null
					RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}

	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());
		bool bDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
		if (nullptr == Brute) return;

		UWorld* World = Brute->GetWorld();

		FVector Center = Brute->GetActorLocation();
		float DetectRadius = 400.f;
		if (bDetected)
			DetectRadius = 1000.f;
		//FVector DetectSize = FVector(100.0f, 800.0f, 10.0f);

		if (nullptr == World) return;
		TArray<FOverlapResult> OverlapResults;
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Brute);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			//FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(),
			ECollisionChannel::ECC_GameTraceChannel3,
			//FCollisionShape::MakeCapsule(20.f, 400.f),
			FCollisionShape::MakeSphere(DetectRadius),
			//FCollisionShape::MakeBox(DetectSize),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

		if (bResult)
		{
			for (auto OverlapResult : OverlapResults)
			{
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, PlayerCharacter);
						BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, PlayerCharacter->GetActorLocation());

						// Show radius
						DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						DrawDebugLine(World, Brute->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller

				else if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, Cabinet);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, true);
						}
						else
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
						}
					}
				}
				else if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, Wardrobe);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, true);
						}
						else
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
						}
					}
				}
				else
				{
					// set null
					BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}

	/*
	if (ReaperAI)
	{
		Creature = ReaperAI->GetPawn();
		bDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::CanSeePlayer);
	}

	if (RunnerAI)
	{
		Creature = RunnerAI->GetPawn();
	}

	// auto Creature = Controller->GetPawn();
	if (nullptr == Creature) return;

	UWorld* World = Creature->GetWorld();
	//FVector Center = ControllingPawn->GetActorLocation() + ControllingPawn->GetActorForwardVector() * 300.0f;
	FVector Center = Creature->GetActorLocation();
	float DetectRadius = 300.f;
	if (bDetected)
		DetectRadius = 800.f;
	//FVector DetectSize = FVector(100.0f, 800.0f, 10.0f);

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, Creature);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		//FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(),
		ECollisionChannel::ECC_GameTraceChannel3,
		//FCollisionShape::MakeCapsule(20.f, 400.f),
		FCollisionShape::MakeSphere(DetectRadius),
		//FCollisionShape::MakeBox(DetectSize),
		CollisionQueryParam
	);
	// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
			{
				if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
				{
					if (ReaperAI)
					{
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, PlayerCharacter);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, PlayerCharacter->GetActorLocation());
					}

					if (RunnerAI)
					{
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, PlayerCharacter);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, PlayerCharacter->GetActorLocation());
					}
					// Show radius
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
					//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
					//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

					DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, Creature->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
					return;
				}
			}
			// Chack if a controller is player's controller

			else if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
			{
				if (Cabinet->bIsHiding && (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn))
				{
					if (ReaperAI)
					{
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Cabinet);
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
					}

					if (RunnerAI)
					{
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, Cabinet);
						RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
					}
				}
			}
			else if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
			{
				if (Wardrobe->bIsHiding && (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn))
				{
					if (ReaperAI)
					{
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, Wardrobe);
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
					}

					if (RunnerAI)
					{
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, Wardrobe);
						RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
					}
				}
			}
			else
			{
				// set null
				if(ReaperAI) 
					ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);

				if(RunnerAI)
					RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
			}
		}
	}
	else
	{
		// set null
		if (ReaperAI)
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);

		if (RunnerAI)
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
	}

	//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
	//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	*/
}