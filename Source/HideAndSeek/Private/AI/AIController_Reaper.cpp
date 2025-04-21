// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/AIController_Reaper.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AI/Reaper_cpp.h"

// 블랙보드 데이터를 블루프린트로부터 읽어올 수 있도록 바인딩할 키를 지정해줌줌.
const FName AAIController_Reaper::SealStatus(TEXT("SealStatus"));

// Constructor
AAIController_Reaper::AAIController_Reaper()
{
	// AI Controller는 Tick 비활성화할 것임. Actor만 해도 충분함.
	PrimaryActorTick.bCanEverTick = false;

	// AI Perception의 세부 데이터 초기화.
	AISightRadius = 1600.0f;
	AISightAge = 0.5f;
	AILoseSightRadius = 1700.0f;
	AIFieldOfView = 90.0f;
	AILastSeenLocation = 1200.0f;
	NoLightSightRadius = 800.0f;
	CurrentStatus = Sealed::Sealed;
}

void AAIController_Reaper::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto const player = Cast<AHorrorGameCharacter>(Actor))
	{
		FTimerHandle Timer;
		if (Stimulus.WasSuccessfullySensed()) // 플레이어 감지했을 경우
		{
			// 플레이어 감지 시 블랙보드에 true 값 넣어줌.
			FVector PlayerLocation = player->GetActorLocation();
			FVector ReaperLocation = GetPawn()->GetActorLocation();
			ReaperLocation.Z = 0.f;
			PlayerLocation.Z = 0.f;
			float Dist = FMath::Abs(FVector::Distance(ReaperLocation, PlayerLocation));
			
			// 나중에 광원 종류 및 On 여부에 따라 감지 거리가 다르게 설정해야함
			GetWorld()->GetTimerManager().ClearTimer(Timer);

			// 능력의 봉인이 해제된 상태라면(즉 플레이어가 오브젝트 3개 이상 먹은 상태라면)
			if (CurrentStatus == Sealed::Unsealed || CurrentStatus == Sealed::TwoUnsealed) 
			{
				Cast<AReaper_cpp>(GetPawn())->SetPlayerWatch(true);
			}
			else
			{
				// 플레이어가 숨어있는 상태면, TargetKey 제거
				if (player->GetIsHiding())
				{
					GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
				}
				else
				{
					// 라이터든 플래시든 켰을 때만 감지
					if (player->bIsLightOn)
					{
						GetBlackboardComponent()->SetValueAsObject(TargetKey, player);
						GetBlackboardComponent()->SetValueAsVector(TargetLocation, player->GetActorLocation());
					}
					else
					{
						// 봉인시 800, 그 외 1200 이하 거리 이내에 있으면 어느 상황에서든 감지
						if (Dist <= NoLightSightRadius)
						{
							GetBlackboardComponent()->SetValueAsObject(TargetKey, player);
							GetBlackboardComponent()->SetValueAsVector(TargetLocation, player->GetActorLocation());
						}
					}
				}
			}
		}
		else // 플레이어 감지가 끝난 경우
		{
			GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
				GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
			}), 1.f, false, 5.f);
			if (CurrentStatus == Sealed::Unsealed)
			{
				Cast<AReaper_cpp>(GetPawn())->SetPlayerWatch(false);
			}
		}
	}
}

void AAIController_Reaper::SetCurrentSealStatus(Sealed Status)
{
	CurrentStatus = Status;
	
	// 첫 봉인이 풀린 경우, 조명 아이템을 쓰지 않았을 때의 감지 범위를 1200으로 늘림.
	if (CurrentStatus == Sealed::OneUnsealed)
	{
		NoLightSightRadius = 1200.0f;
	}

	GetBlackboardComponent()->SetValueAsEnum(SealStatus, static_cast<uint8>(CurrentStatus));
}

Sealed AAIController_Reaper::GetCurrentSealStatus()
{
	return CurrentStatus;
}

void AAIController_Reaper::SetupBlackboardAndBehaviorTree()
{
	Super::SetupBlackboardAndBehaviorTree();

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Assets/AI/Reaper/BB_Reaper"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BB Access Completed!"))
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Assets/AI/Reaper/BT_Reaper"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BT Access Completed!"))
	}
}