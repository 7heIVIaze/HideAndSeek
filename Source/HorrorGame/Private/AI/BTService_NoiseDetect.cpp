// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTService_NoiseDetect.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "HorrorGame/HorrorGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Items/TimerProjectile_cpp.h"
#include "Items/ThrownTimer_cpp.h"
#include "Alarm.h"

UBTService_NoiseDetect::UBTService_NoiseDetect()
{
	NodeName = TEXT("NoiseDetect");
	Interval = 0.5f;
}

void UBTService_NoiseDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController)) // 해당 AI Controller가 Reaper Controller일 경우
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());
		bool bDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected); // 이전에 소리를 감지했는지 확인

		if (nullptr == Reaper)
		{
			return;
		}

		UWorld* World = Reaper->GetWorld();
		FVector vCenter = Reaper->GetActorLocation();
		float fDetectRadius = 1600.f; // 청각 감지 범위 1600(Default)임.

		if (nullptr == World) return;

		TArray<FOverlapResult> OverlapResults;
		
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Reaper);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			vCenter, // Reaper의 현재 위치를 중심으로
			FQuat::Identity, // 아무런 회전값 없이
			ECollisionChannel::ECC_GameTraceChannel6, // 플레이어만 감지하는 채널을 활용해서
			FCollisionShape::MakeSphere(fDetectRadius), // 반지름 1600인 Sphere Component를 생성함
			CollisionQueryParam
		);

		if (bResult) // 어떠한 감지된 것이 존재할 경우
		{
			for (FOverlapResult OverlapResult : OverlapResults) // 여러 감지된 것들 중 하나씩 확인해봄
			{
				//if (auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor())) // 감지된 액터가 Timer Projectile(던져진 타이머)일 경우
				if (auto TimerActor = Cast<AThrownTimer_cpp>(OverlapResult.GetActor())) // 감지된 액터가 Timer Projectile(던져진 타이머)일 경우
				{

					if (TimerActor->GetPlaySound()) // 현재 타이머가 울리고 있다면
					{
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true); // Noise Detect 값을 true로 설정하고
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, TimerActor->GetActorLocation()); // 해당 타이머의 위치를 타겟 로케이션으로 줌(그쪽으로 무빙하게)
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::NoiseTargetKey, TimerActor); // 혹시나 하여 TimerActor를 패트롤 타겟으로 해서 그쪽으로 움직이게
						// 타켓 키로 설정하면 Trace가 활성화되므로 이게 맞음
					}
				}

				else if (auto Alarm = Cast<AAlarm>(OverlapResult.GetActor())) // 감지된 액터가 경보기일 경우엔
				{
					if (Alarm->bIsAlarmRing) // 현재 경보기가 울리고 있으면
					{
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Alarm->GetActorLocation());
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::NoiseTargetKey, Alarm);
					}
				}	
				
				else if (auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor())) // 만약에 감지된 액터가 Player라면
				{
					if (Player->GetIsSprinting()) // Player가 뛰고 있는 상황에만
					{
						if (!bDetected) // 이전에 감지된 상태가 아니면
						{
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true); // 감지 여부를 true로 하고
							//ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::SprintDetected, true); // 감지 여부를 true로 하고
							ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Player->GetActorLocation()); // 해당 위치로 움직이게 설정
						}
					}

					if (Player->GetIsScreaming()) // 플레이어가 패닉으로 소리를 지를 때
					{
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
						//ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::SprintDetected, true);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Player->GetActorLocation());
					}
				}
			}
		}
		//else // 아무것도 감지되지 않은 상태이면
		//{
		//	ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, false); // false로 설정함
		//}
	}

	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController)) // AI Controller가 Runner Controller일 경우
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());
		bool bDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected); // 이전에 소리를 감지했는지 확인

		if (nullptr == Runner)
		{
			return;
		}

		UWorld* World = Runner->GetWorld();
		FVector vCenter = Runner->GetActorLocation();
		float fDetectRadius = 1600.f; // 소리 감지 범위는 1600(Default)

		if (nullptr == World) return;

		TArray<FOverlapResult> OverlapResults;
		//bool bDetected = AIController->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected, true)
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Runner);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults, // 감지된 결과를 저장할 변수
			vCenter, // Runner의 위치를 중심으로
			FQuat::Identity, // 어떠한 회전값도 없이
			ECollisionChannel::ECC_GameTraceChannel6, // Player를 감지할 수 있는 채널을 활용해서
			FCollisionShape::MakeSphere(fDetectRadius), // 반지름이 1600인 Sphere Component 생성
			CollisionQueryParam
		);

		if (bResult) // 감지된 결과가 있으면
		{
			for (FOverlapResult OverlapResult : OverlapResults) // 여러 값들 중에서 하나씩 확인
			{
				//if (auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor())) // 감지된 액터가 TimerProjectile(던져진 타이머)라면
				if (auto TimerActor = Cast<AThrownTimer_cpp>(OverlapResult.GetActor())) // 감지된 액터가 TimerProjectile(던져진 타이머)라면
				{
					if (TimerActor->GetPlaySound()) // 현재 타이머가 울리고 있으면 소리를 감지하고 타이머 위치로 이동하게 설정
					{
						RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, TimerActor->GetActorLocation());
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::NoiseTargetKey, TimerActor); // 혹시나해서 패트롤 타겟을 Timer로 설정
					}
				}

				else if (auto Alarm = Cast<AAlarm>(OverlapResult.GetActor())) // 그게 아니라 감지된 액터가 경보기일 경우
				{
					if (Alarm->bIsAlarmRing) // 경보기가 울릴 경우에만 소리를 감지하고 경보기 위치로 이동함
					{
						RunnerAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
						RunnerAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Alarm->GetActorLocation());
						RunnerAI->GetBlackboard()->SetValueAsObject(ACreatureAI::NoiseTargetKey, Alarm);
					}
				}
				
				else if (auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor())) // 그 외 Player가 감지된 경우엔
				{
					if (Player->GetIsSprinting()) // 플레이어가 뛰고 있는 상황에만 감지
					{
						if (!bDetected) // 그것도 이전에 아무것도 감지된 것이 없는 상태일 경우에
						{
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
							//RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::SprintDetected, true);
							RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, Player->GetActorLocation());
						}
					}

					if (Player->GetIsScreaming()) // 플레이어가 패닉으로 소리를 지를 때
					{
						RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
						//RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::SprintDetected, true);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, Player->GetActorLocation());
					}
				}
			}
		}
		//else // 아무것도 감지된 것이 없을 때 false로 설정.
		//{
		//	RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, false);
		//}
	}

	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController)) // AI Controller가 Brute Controller일 경우에
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());
		bool bDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::NoiseDetected); // 이전에 소리를 감지했는지 확인

		if (nullptr == Brute)
		{
			return;
		}

		UWorld* World = Brute->GetWorld();
		FVector vCenter = Brute->GetActorLocation();
		float fDetectRadius = Brute->GetNoiseRange(); // Brute는 소리를 잘 듣는 설정이므로 다른 개체들과는 다르게 소리 듣는 범위가 다름
		// default: 2000, awaken: 2400

		if (nullptr == World) return;

		TArray<FOverlapResult> OverlapResults;
		
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, Brute);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults, // 감지된 개체들을 담을 변수
			vCenter, // Brute의 위치를 중심으로
			FQuat::Identity, // 어떠한 회전값도 주지 않고
			ECollisionChannel::ECC_GameTraceChannel6, // Player를 감지할 수 있는 채널을 활용해서
			FCollisionShape::MakeSphere(fDetectRadius), // Brute의 소리 감지 범위 만큼의 Sphere Component 생성
			CollisionQueryParam
		);

		if (bResult) // Collision에 어떤 객체가 충돌한 상황이라면
		{
			for (FOverlapResult OverlapResult : OverlapResults) // 감지된 객체들을 하나씩 확인
			{
				//if (auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor())) // 감지된 액터가 던져진 타이머일 경우
				if (auto TimerActor = Cast<AThrownTimer_cpp>(OverlapResult.GetActor())) // 감지된 액터가 던져진 타이머일 경우
				{
					if (TimerActor->GetPlaySound()) // 해당 타이머가 울리고 있는 경우에 감지
					{
						BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, true);
						BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, TimerActor->GetActorLocation());
						BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::NoiseTargetKey, TimerActor);
					}
				}

				else if (auto Alarm = Cast<AAlarm>(OverlapResult.GetActor())) // 그게 아닌 감지된 액터가 경보기일 경우
				{
					if (Alarm->bIsAlarmRing) // 경보기가 울리고 있는 경우에만 감지하여 이동하게 설정
					{
						BruteAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
						BruteAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Alarm->GetActorLocation());
						BruteAI->GetBlackboard()->SetValueAsObject(ACreatureAI::NoiseTargetKey, Alarm);
					}
				}

				else if (auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor())) // 감지된 액터가 Player일 경우
				{
					if (Player->GetIsSprinting()) // Player가 뛰고 있는 상황에만 감지하여 이동하게 설정
					{
						if (!bDetected) // 이전에 감지된 것이 없는 경우에만 감지하게 + 처음 뛴 위치로만 이동하게 설정
						{
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, true);
							//BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::SprintDetected, true);
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, Player);
							BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, Player->GetActorLocation());
						}
					}
					else // 만약 Player가 뛰지 않고
					{
						if ((Player->GetVelocity().Size() > 0.f && !Player->bIsCrouch) && !bDetected) // 움크리지도 않고 그냥 걸은 상태라면
						{
							if (Brute->bIsRangeChange) // 이미 Awaken 상태라면 소리를 감지하고 Player의 위치로 이동하게 설정
							{
								BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, true);
								//BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::SprintDetected, true);
								BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, Player);
								BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, Player->GetActorLocation());
							}

							else // Normal 상태라면 Awaken 상태로 먼저 변경하게 설정 
							{
								Brute->BroadCastChangeNoiseRange();
							}
						}
					}

					if (Player->GetIsScreaming()) // 플레이어가 패닉으로 소리를 지를 때
					{
						BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, true);
						//BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::SprintDetected, true);
						BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, Player);
						BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, Player->GetActorLocation());
					}

					/*if ((Player->bIsBellSoundOn)) // 방울을 울리는 상황에도 감지하게 설정하려 했으나, 밸런스 측면에서 삭제하기로 결정
					{
						BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, true);
						BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, Player->GetActorLocation());
					}*/
				}
				//else // Overlap된 객체가 존재하지만 위의 경우에 모두 해당 안 될 때는 false로 바꿔줘서 초기화시켜줌
				//{
				//	BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, false);
				//}
			}
		}
		//else // 아무 것도 감지된 객체가 없으면 감지여부를 false로 설정함 <= 이 부분과 관련해서 뭔가 문제 발생하는거 같음. 수정 필요
		//{
		//	BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, false);
		//}
	}

	/*
	if (ReaperAI)
	{
		Creature = ReaperAI->GetPawn();
		bDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);
	}
	
	if (RunnerAI)
	{
		Creature = RunnerAI->GetPawn();
		bDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected);
	}

	if (nullptr == Creature)
	{
		return;
	}

	UWorld* World = Creature->GetWorld();
	FVector vCenter = Creature->GetActorLocation();
	float fDetectRadius = 800.f;
	// bool detected = AIController->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);

	if (nullptr == World) return;

	TArray<FOverlapResult> OverlapResults;
	//bool bDetected = AIController->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected, true)
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, Creature);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		vCenter,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(fDetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		//ATimerProjectile_cpp* TimerActor = nullptr;
		//AHorrorGameCharacter* Player = nullptr;
		for (FOverlapResult OverlapResult : OverlapResults)
		{
			auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor());
			auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor());
			if (TimerActor)
			{
				//auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor());
				
				if (TimerActor->GetPlaySound())
				{
					if (ReaperAI)
					{
						ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, TimerActor->GetActorLocation());
					}

					if (RunnerAI)
					{
						RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, TimerActor->GetActorLocation());
					}
				}
			}
			else if (Player && Player->GetController()->IsPlayerController())
			{
				//auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor());

				if ((nullptr != Player) && (Player->GetController()->IsPlayerController()))
				{
					if((Player->GetIsSprinting()))
					{
						if (!bDetected)
						{
							if (ReaperAI)
							{
								ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
								ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Player->GetActorLocation());
							}

							if (RunnerAI)
							{
								RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
								RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, Player->GetActorLocation());
							}
						}
					}

					if ((Player->bIsBellSoundOn))
					{
						if (ReaperAI)
						{
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, true);
							ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, Player->GetActorLocation());
						}

						if (RunnerAI)
						{
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, true);
							RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, Player->GetActorLocation());
						}
					}
				}
			}
		}
	}
	else
	{
		if(ReaperAI)
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, false);

		if(RunnerAI)
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, false);
	}
	*/
}