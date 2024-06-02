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
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "AI/Shadow_cpp.h"

// 블랙보드 데이터를 블루프린트로부터 읽어올 수 있도록 바인딩해줌.
const FName AAIController_Shadow::PatrolTargetKey(TEXT("PatrolTarget"));
const FName AAIController_Shadow::NoiseTargetKey(TEXT("NoiseTargetKey"));
const FName AAIController_Shadow::HomePosKey(TEXT("HomePos"));
const FName AAIController_Shadow::PatrolPosKey(TEXT("PatrolPos"));
const FName AAIController_Shadow::TargetKey(TEXT("Target"));
const FName AAIController_Shadow::TargetLocation(TEXT("TargetLocation"));
const FName AAIController_Shadow::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AAIController_Shadow::NoiseDetected(TEXT("NoiseDetected"));
const FName AAIController_Shadow::Stunned(TEXT("Stunned"));
const FName AAIController_Shadow::LockerLighting(TEXT("LockerLighting"));
const FName AAIController_Shadow::LockerTargetKey(TEXT("LockerTarget"));

// Constructor
AAIController_Shadow::AAIController_Shadow()
{
	// AI Controller는 Tick 비활성화. Actor만 해도 충분,
	PrimaryActorTick.bCanEverTick = false;

	// 블랙보드 컴포넌트를 블루프린트 클래스로부터 가져와 생성함.
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Assets/AI/Shadow/BB_Shadow"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	// 비헤이비어트리 컴포넌트를 블루프린트 클래스로부터 가져와 생성함.
	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Assets/AI/Shadow/BT_Shadow"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	// AI Perception 설정
	SetPerception();
}

void AAIController_Shadow::BeginPlay()
{
	Super::BeginPlay();

}

void AAIController_Shadow::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		RunBehaviorTree(BTAsset);
	}
	/*if (BlackboardComp)
	{
		BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);
	}*/
}

// AI가 Destroy될 경우 비헤이비어트리를 정리하고 Destroy되도록 하는 함수
void AAIController_Shadow::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComp == nullptr) return;

	BehaviorTreeComp->StopTree(EBTStopMode::Safe);
}

UBlackboardComponent* AAIController_Shadow::GetBlackboard() const
{
	return BlackboardComp;
}

// 플레이어가 AI Perception의 주 감각에 의해 감지될 경우 처리할 함수
void AAIController_Shadow::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	// 플레이어의 감지가 끝났을 때 5초간 그 값을 유지하기 위해 사용할 타이머 변수
	FTimerHandle Timer;

	if (Stimulus.WasSuccessfullySensed()) // AI Perception을 통해 감지했을 경우
	{
		// 감지된 액터가 플레이어일 경우
		if (auto const Player = Cast<AHorrorGameCharacter>(Actor))
		{
			// 거리 비교를 위해 감지한 플레이어와의 거리를 가져옴.
			float Distance = this->GetPawn()->GetDistanceTo(Player);

			if (Player->GetIsHiding()) // 플레이어를 감지했는데 숨은 경우에는
			{
				// Target Key에 null 포인터를 저장시킴.
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}
			// 그것이 아닌 경우엔 감지된 것으로 확정
			else
			{
				if (Player->bIsFlashLightOn || Player->bIsCigarLightOn) // AI의 감지 범위가 1200이므로 불을 켰는지 체크
				{
					// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
					if (Distance <= 600.f) // 만약 600 범위 내에서 감지한 경우라면 사라짐.
					{
						StopAI();
						Cast<AShadow_cpp>(GetPawn())->Exorcism();
					}
					// 불을 켠 경우 최대 감지 범위까지 감지가 가능하므로 TargetKey에 플레이어를 저장시킴.
					else
					{
						GetBlackboard()->SetValueAsObject(TargetKey, Player);
						GetBlackboard()->SetValueAsVector(TargetLocation, Player->GetActorLocation());
					}
				}

				else // 아닐 경우 거리를 비교해서 400 이내에 있을 때만 감지
				{
					if (Distance <= 1200.f)
					{
						// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
						GetBlackboard()->SetValueAsObject(TargetKey, Player);
						GetBlackboard()->SetValueAsVector(TargetLocation, Player->GetActorLocation());
					}
				}
			}
		}
		
	}

	else // 플레이어 감지가 끝난 경우
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
			GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}), 1.f, false, 5.f);
	}
}

// AI Perception 설정하는 함수. Constructor에서 호출할 것
void AAIController_Shadow::SetPerception()
{
	// AI Perception의 기능 중 Sight Config(시야 감지) 컴포넌트를 생성함.
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	// 생성한 Sight Config Object를 초기화해줌.
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	// AI가 적, 중립, 동맹 관계없이 시각으로 감지할 수 있도록 설정해줌.
	// 팀 설정을 안 해놨음. 해놓을지 고민 중
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AI Perception의 주 감각을 시각(Sight Config)으로 설정하고, 감지될 때마다 수행할 콜백 함수를 바인딩해줌.
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Shadow::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIController_Shadow::SetStunned(bool value)
{
	/*if (bIsStunned != value)
	{
		bIsStunned = value;
	}*/
	GetBlackboard()->SetValueAsBool(Stunned, value);
}