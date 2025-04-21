// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTService_NoiseDetect.h"
#include "AI/AIController_Brute.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "AI/Brute_cpp.h"
#include "Player/HorrorGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/TimerProjectile_cpp.h"
#include "Furniture/Alarm.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

UBTService_NoiseDetect::UBTService_NoiseDetect()
{
	NodeName = TEXT("NoiseDetect");
	Interval = 0.5f;
}

void UBTService_NoiseDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(AIController)) // AI Controller가 Brute Controller일 경우에
	{
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());
		bool bDetected = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::NoiseDetected); // 이전에 소리를 감지했는지 확인
		bool bRangeChanged = false;
		
		// Brute는 소리 감지 범위가 유일하게 달라지는 개체이므로 변화되었는지 체크함.
		if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(YokaiAI))
		{
			bRangeChanged = BruteAI->GetBlackboardComponent()->GetValueAsBool(AAIController_Brute::ChangeDetectRange); // 이전에 소리를 감지했는지 확인
		}

		if (nullptr == Yokai)
		{
			return;
		}

		bool bIsChasing = Yokai->bIsChase;
		UWorld* World = Yokai->GetWorld();
		FVector Center = Yokai->GetActorLocation();

		float DetectRadius = BaseRadius;
		
		// Brute는 소리를 잘 듣는 설정이므로 다른 개체들과는 다르게 소리 듣는 범위가 다름
		if (bRangeChanged)
		{
			DetectRadius = ExtendedRadius;
		}
		//fDetectRadius = Brute->GetNoiseRange(); 
		// (대부분): default: 1600 / (Brute): default: 2000, awaken: 2400

		if (nullptr == World) return;

		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그를 가진 액터만 검색하도록 설정함.
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Yokai);
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults, // 감지된 개체들을 담을 변수
			Center, // 요괴의 위치를 중심으로
			FQuat::Identity, // 어떠한 회전값도 주지 않고
			ECollisionChannel::ECC_GameTraceChannel6, // Player를 감지할 수 있는 채널을 활용해서
			FCollisionShape::MakeSphere(DetectRadius), // 요괴의 소리 감지 범위 만큼의 Sphere Component 생성
			CollisionQueryParam
		);

		if (!bIsChasing)
		{
			if (bResult) // Collision에 어떤 객체가 충돌한 상황이라면
			{
				for (FOverlapResult OverlapResult : OverlapResults) // 감지된 객체들을 하나씩 확인
				{
					// 감지된 액터가 던져진 타이머일 경우
					if (auto TimerActor = Cast<ATimerProjectile_cpp>(OverlapResult.GetActor()))
					{
						// 해당 타이머가 울리고 있는 경우에 감지
						if (TimerActor->GetIsSoundPlaying())
						{
							// Brute의 경우를 체크해서 범위가 달라진 경우 원상복귀시키도록 함.
							if (bRangeChanged)
							{
								if (ABrute_cpp* Brute = Cast<ABrute_cpp>(Yokai))
								{
									Brute->ChangeNoiseRange(false);
								}
							}

							YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, true);
							YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, TimerActor->GetActorLocation());
							YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::NoiseTargetKey, TimerActor);
						}
					}

					// 그게 아닌 감지된 액터가 경보기일 경우
					else if (auto Alarm = Cast<AAlarm>(OverlapResult.GetActor()))
					{
						// 경보기가 울리고 있는 경우에만 감지하여 이동하게 설정
						if (Alarm->bIsAlarmRing)
						{
							// Brute의 경우를 체크해서 범위가 달라진 경우 원상복귀시키도록 함.
							if (bRangeChanged)
							{
								if (ABrute_cpp* Brute = Cast<ABrute_cpp>(Yokai))
								{
									Brute->ChangeNoiseRange(false);
								}
							}
							YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, true);
							YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, Alarm->GetActorLocation());
							YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::NoiseTargetKey, Alarm);
						}
					}

					// 감지된 액터가 Player일 경우
					else if (auto Player = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
					{
						// Player가 뛰고 있는 상황에만 감지하여 이동하게 설정
						if (Player->GetIsSprinting())
						{
							// 이전에 감지된 것이 없는 경우에만 감지하게 + 처음 뛴 위치로만 이동하게 설정
							if (!bDetected)
							{
								YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, true);
								YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, Player);
								YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, Player->GetActorLocation());
							}
						}
						// 만약 Player가 뛰지 않고
						else 
						{
							// Brute의 경우일 때, 걷더라도 감지가 가능하도록 체크함.
							if (ABrute_cpp* Brute = Cast<ABrute_cpp>(Yokai))
							{
								if ((Player->GetVelocity().Size() > 0.f && !Player->bIsCrouch) && !bDetected) // 움크리지도 않고 그냥 걸은 상태라면
								{
									// 이미 Awaken 상태라면 소리를 감지하고 Player의 위치로 이동하게 설정
									if (Brute->bCalledRangeChange)
									{
										YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, true);
										YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, Player);
										YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, Player->GetActorLocation());
									}

									// Normal 상태라면 Awaken 상태로 먼저 변경하게 설정 
									else 
									{
										Brute->ChangeNoiseRange(true);
									}
								}
							}
						}

						// 플레이어가 패닉으로 소리를 지를 때
						if (Player->GetIsScreaming()) 
						{
							YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, true);
							YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, Player);
							YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, Player->GetActorLocation());
						}
					}
				}
			}
			// 아무 것도 감지된 객체가 없으면 감지여부를 false로 설정함 <= 이 부분과 관련해서 뭔가 문제 발생하는거 같음. 수정 필요
			else 
			{
				// Brute의 경우 감지 범위 초기화.
				if (ABrute_cpp* Brute = Cast<ABrute_cpp>(Yokai))
				{
					Brute->ChangeNoiseRange(false);
				}
				YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::NoiseDetected, false);
			}
		}

		if (bIsDebug)
		{
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		}
	}
}