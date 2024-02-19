// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/AIController_Brute.h"
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

const FName AAIController_Brute::PatrolTargetKey(TEXT("PatrolTarget"));
const FName AAIController_Brute::NoiseTargetKey(TEXT("NoiseTargetKey"));
const FName AAIController_Brute::HomePosKey(TEXT("HomePos"));
const FName AAIController_Brute::PatrolPosKey(TEXT("PatrolPos"));
const FName AAIController_Brute::TargetKey(TEXT("Target"));
const FName AAIController_Brute::TargetLocation(TEXT("TargetLocation"));
const FName AAIController_Brute::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AAIController_Brute::NoiseDetected(TEXT("NoiseDetected"));
//const FName AAIController_Brute::SprintDetected(TEXT("SprintDetected"));
const FName AAIController_Brute::Stunned(TEXT("Stunned"));
const FName AAIController_Brute::LockerLighting(TEXT("LockerLighting"));

AAIController_Brute::AAIController_Brute()
{
	PrimaryActorTick.bCanEverTick = false;
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Assets/AI/Brute/BB_Brute"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Assets/AI/Brute/BT_Brute"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	SetPerception();
}

void AAIController_Brute::BeginPlay()
{
	Super::BeginPlay();
	/*RunBehaviorTree(BTAsset);
	Behavior_Tree_Component->StartTree(*BTAsset);*/
	// BlackboardComp->InitializeBlackboard(*BBAsset);
}

void AAIController_Brute::OnPossess(APawn* InPawn)
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

void AAIController_Brute::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComp == nullptr) return;

	BehaviorTreeComp->StopTree(EBTStopMode::Safe);
}


UBlackboardComponent* AAIController_Brute::GetBlackboard() const
{
	return BlackboardComp;
}

void AAIController_Brute::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	FTimerHandle Timer;
	if (Stimulus.WasSuccessfullySensed()) // 플레이어 감지했을 경우
	{
		if (auto const Player = Cast<AHorrorGameCharacter>(Actor))
		{
			/*FVector PlayerLocation = Player->GetActorLocation();
			FVector BruteLocation = this->GetPawn()->GetActorLocation();*/
			float Distance = this->GetPawn()->GetDistanceTo(Player);

			if (Player->GetIsHiding()) // 감지했는데 숨은 경우
			{
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}
			else
			{
				if (Player->bIsFlashLightOn || Player->bIsCigarLightOn) // AI의 감지 범위가 1200이므로 불을 켰는지 체크
				{
					// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
					GetBlackboard()->SetValueAsObject(TargetKey, Player);
					GetBlackboard()->SetValueAsVector(TargetLocation, Player->GetActorLocation());
				}

				else // 아닐 경우 거리를 비교해서 400 이내에 있을 때만 감지
				{
					if (Distance <= NoLightSightRadius)
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

void AAIController_Brute::SetPerception()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = AILastSeenLocation;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Brute::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIController_Brute::SetStunned(bool value)
{
	/*if (bIsStunned != value)
	{
		bIsStunned = value;
	}*/
	GetBlackboard()->SetValueAsBool(Stunned, value);
}