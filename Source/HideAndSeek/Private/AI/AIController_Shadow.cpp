// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/AIController_Shadow.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/HideObject.h"
#include "AI/Shadow_cpp.h"

// Constructor
AAIController_Shadow::AAIController_Shadow()
{
	// AI Controller는 Tick 비활성화. Actor만 해도 충분,
	PrimaryActorTick.bCanEverTick = false;

	// AI Perception의 세부 데이터 초기화.
	AISightRadius = 1200.f;
	AISightAge = 0.5f;
	AILoseSightRadius = 1700.f;
	AIFieldOfView = 60.0f;
	AILastSeenLocation = 2000.f;
	NoLightSightRadius = 800.f;
}

// 플레이어가 AI Perception의 주 감각에 의해 감지될 경우 처리할 함수
void AAIController_Shadow::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	// 플레이어의 감지가 끝났을 때 5초간 그 값을 유지하기 위해 사용할 타이머 변수
	FTimerHandle Timer;

	// AI Perception을 통해 감지했을 경우
	if (Stimulus.WasSuccessfullySensed())
	{
		// 감지된 액터가 플레이어일 경우
		if (auto const Player = Cast<AHorrorGameCharacter>(Actor))
		{
			// 거리 비교를 위해 감지한 플레이어와의 거리를 가져옴.
			float Distance = this->GetPawn()->GetDistanceTo(Player);

			// 플레이어를 감지했는데 숨은 경우에는
			if (Player->GetIsHiding())
			{
				// Target Key에 null 포인터를 저장시킴.
				GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
			}
			// 그것이 아닌 경우엔 감지된 것으로 확정
			else
			{
				// AI의 감지 범위가 1200이므로 불을 켰는지 체크
				if (Player->bIsLightOn)
				{
					// 만약 600 범위 내에서 감지한 경우라면 사라짐.
					if (Distance <= 600.f)
					{
						StopAI();
						Cast<AShadow_cpp>(GetPawn())->Exorcism();
					}
					// 불을 켠 경우 최대 감지 범위까지 감지가 가능하므로 TargetKey에 플레이어를 저장시킴.
					else
					{
						GetBlackboardComponent()->SetValueAsObject(TargetKey, Player);
						GetBlackboardComponent()->SetValueAsVector(TargetLocation, Player->GetActorLocation());
					}
				}

				else // 아닐 경우 거리를 비교해서 800 이내에 있을 때만 감지
				{
					if (Distance <= NoLightSightRadius)
					{
						GetBlackboardComponent()->SetValueAsObject(TargetKey, Player);
						GetBlackboardComponent()->SetValueAsVector(TargetLocation, Player->GetActorLocation());
					}
				}
			}
		}
		
	}

	// 플레이어 감지가 끝난 경우
	else
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
			GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
			}), 1.f, false, 5.f);
	}
}

void AAIController_Shadow::SetupBlackboardAndBehaviorTree()
{
	Super::SetupBlackboardAndBehaviorTree();

	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Assets/AI/Shadow/BB_Shadow"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// 비헤이비어트리 컴포넌트를 블루프린트 클래스로부터 가져와 생성함.
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Assets/AI/Shadow/BT_Shadow"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}