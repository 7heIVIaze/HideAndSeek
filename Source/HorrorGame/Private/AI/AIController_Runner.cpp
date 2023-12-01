// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/AIController_Runner.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "HorrorGame/HorrorGameCharacter.h"

const FName AAIController_Runner::PatrolTargetKey(TEXT("PatrolTarget"));
const FName AAIController_Runner::NoiseTargetKey(TEXT("NoiseTargetKey"));
const FName AAIController_Runner::HomePosKey(TEXT("HomePos"));
const FName AAIController_Runner::PatrolPosKey(TEXT("PatrolPos"));
const FName AAIController_Runner::TargetKey(TEXT("Target"));
const FName AAIController_Runner::TargetLocation(TEXT("TargetLocation"));
const FName AAIController_Runner::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AAIController_Runner::NoiseDetected(TEXT("NoiseDetected"));
//const FName AAIController_Runner::SprintDetected(TEXT("SprintDetected"));
const FName AAIController_Runner::Stunned(TEXT("Stunned"));
const FName AAIController_Runner::LockerLighting(TEXT("LockerLighting"));

AAIController_Runner::AAIController_Runner()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Assets/AI/Runner/BB_Runner"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Assets/AI/Runner/BT_Runner"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));

	SetPerception();
}

void AAIController_Runner::BeginPlay()
{
	Super::BeginPlay();
	/*RunBehaviorTree(BTAsset);
	Behavior_Tree_Component->StartTree(*BTAsset);*/
}

void AAIController_Runner::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		RunBehaviorTree(BTAsset);
	}

}

void AAIController_Runner::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComp == nullptr) return;

	BehaviorTreeComp->StopTree(EBTStopMode::Safe);
}

UBlackboardComponent* AAIController_Runner::GetBlackboard() const
{
	return BlackboardComp;
}
//void ACreatureAI::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

void AAIController_Runner::UpdatePerception(const TArray<AActor*>& Actors)
{
	if (BlackboardComp == nullptr)
		return;

	int size = Actors.Num();
	// bool catched = false;

	for (int i = 0; i < size; ++i)
	{
		AHorrorGameCharacter* player = Cast<AHorrorGameCharacter>(Actors[i]);
		if (player != nullptr)
		{

			/*if (player->GetIsHiding())
			{
				GetAIPerceptionComponent()->ForgetAll();
			}*/

			player = Cast<AHorrorGameCharacter>(BlackboardComp->GetValueAsObject(TargetKey));

			if (player == nullptr)
			{
				BlackboardComp->SetValueAsObject(TargetKey, Actors[i]);
				BlackboardComp->SetValueAsVector(TargetLocation, Actors[i]->GetActorLocation());
			}
			else
			{
				BlackboardComp->SetValueAsObject(TargetKey, nullptr);

			}
			break;
		}
	}
}

void AAIController_Runner::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	FTimerHandle Timer;
	if (Stimulus.WasSuccessfullySensed()) // 플레이어 감지했을 경우
	{
		if (auto const player = Cast<AHorrorGameCharacter>(Actor))
		{
			FVector PlayerLocation = player->GetActorLocation();
			FVector RunnerLocation = GetPawn()->GetActorLocation();
			PlayerLocation.Z = RunnerLocation.Z;
			float Dist = FMath::Abs(FVector::Distance(RunnerLocation, PlayerLocation));
			// 플레이어 감지 시 블랙보드에 true 값 넣어줌.
			// 나중에 광원 종류 및 On 여부에 따라 감지 거리가 다르게 설정해야함
			if (player->GetIsHiding())
			{
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}
			else
			{
				if (player->bIsCigarLightOn || player->bIsFlashLightOn) // 라이터든 플래시든 켰을 때만 감지
				{
					// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
					GetBlackboard()->SetValueAsObject(TargetKey, player);
					GetBlackboard()->SetValueAsVector(TargetLocation, player->GetActorLocation());
				}
				else
				{
					if (Dist <= 2000.f) // 2000 이상 2400 이하 거리 이내에 있으면 어느 상황에서든 감지
					{
						GetBlackboard()->SetValueAsObject(TargetKey, player);
						GetBlackboard()->SetValueAsVector(TargetLocation, player->GetActorLocation());
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

void AAIController_Runner::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	if (nullptr == CurrentPawn)
	{
		return;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is nullptr"));
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		UE_LOG(LogTemp, Warning, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}

void AAIController_Runner::SetPerception()
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Runner::OnTargetDetected);
	// GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACreatureAI::UpdatePerception);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIController_Runner::SetStunned(bool value)
{
	/*if (value)
	{
		bIsStunned = value;
	}*/
	GetBlackboard()->SetValueAsBool(Stunned, value);
}

void AAIController_Runner::EndChase()
{
	UAIPerceptionComponent* AIPerceptionComponent = GetAIPerceptionComponent();
	if (AIPerceptionComponent)
	{
		// AI Sight 센스 다시 활성화
		AIPerceptionComponent->RequestStimuliListenerUpdate();
	}
}