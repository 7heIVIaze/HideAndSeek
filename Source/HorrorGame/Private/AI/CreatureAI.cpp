// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/CreatureAI.h"
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
#include "AI/Reaper_cpp.h"

const FName ACreatureAI::PatrolTargetKey(TEXT("PatrolTarget"));
const FName ACreatureAI::HomePosKey(TEXT("HomePos"));
const FName ACreatureAI::PatrolPosKey(TEXT("PatrolPos"));
const FName ACreatureAI::TargetKey(TEXT("Target"));
const FName ACreatureAI::TargetLocation(TEXT("TargetLocation"));
const FName ACreatureAI::CanSeePlayer(TEXT("CanSeePlayer"));
const FName ACreatureAI::NoiseDetected(TEXT("NoiseDetected"));
const FName ACreatureAI::Stunned(TEXT("Stunned"));
const FName ACreatureAI::LockerLighting(TEXT("LockerLighting"));
const FName ACreatureAI::SealStatus(TEXT("SealStatus"));

ACreatureAI::ACreatureAI()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Assets/AI/Reaper/BB_Reaper"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BB Access Completed!"))
	}

	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Assets/AI/Reaper/BT_Reaper"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BT Access Completed!"))
	}
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));

	// Initialize Perception
	SetPerception();
}

void ACreatureAI::BeginPlay()
{
	Super::BeginPlay();

	CurrentStatus = Sealed::Sealed;
	// RunBehaviorTree(BTAsset);
	// Behavior_Tree_Component->StartTree(*BTAsset);
}

//void ACreatureAI::Tick(float DeltaSeconds)
//{
//
//}

void ACreatureAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//UBlackboardComponent* BlackboardComp = Blackboard;
	//if (UseBlackboard(BBAsset, BlackboardComp))
	//{
	//	// Save Current Location in HomePosKey of Blackboard
	//	BlackboardComp->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("AIController couldn't run behavior tree!"))
	//	}
	//}
	/*if (BlackboardComp)
	{
		BlackboardComp->InitializeBlackboard(*BTAsset->BlackboardAsset);
	}*/
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		RunBehaviorTree(BTAsset);
	}
}

UBlackboardComponent* ACreatureAI::GetBlackboard() const
{
	return BlackboardComp;
}
//void ACreatureAI::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

void ACreatureAI::UpdatePerception(const TArray<AActor*>& Actors)
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

			if (player->GetIsHiding())
			{
				GetAIPerceptionComponent()->ForgetAll();
			}

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

void ACreatureAI::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{

	if (auto const player = Cast<AHorrorGameCharacter>(Actor))
	{
		FTimerHandle Timer;
		if (Stimulus.WasSuccessfullySensed()) // 플레이어 감지했을 경우
		{
			// 플레이어 감지 시 블랙보드에 true 값 넣어줌.
			// 나중에 광원 종류 및 On 여부에 따라 감지 거리가 다르게 설정해야함
			GetWorld()->GetTimerManager().ClearTimer(Timer);
			if (CurrentStatus == Sealed::Unsealed) // 능력의 봉인이 해제된 상태라면(즉 플레이어가 오브젝트 3개를 먹은 상태라면)
			{
				Cast<AReaper_cpp>(GetPawn())->SetPlayerWatch(true);
			}
			else
			{
				if (player->GetIsHiding())
				{
					GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
				}
				else
				{
					// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
					GetBlackboard()->SetValueAsObject(TargetKey, player);
					GetBlackboard()->SetValueAsVector(TargetLocation, player->GetActorLocation());
				}
			}
		}
		else // 플레이어 감지가 끝난 경우
		{
			GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}), 1.f, false, 5.f);
			if (CurrentStatus == Sealed::Unsealed)
			{
				Cast<AReaper_cpp>(GetPawn())->SetPlayerWatch(false);
			}
		}
	}
}

void ACreatureAI::OnRepeatTimer()
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

void ACreatureAI::SetPerception()
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ACreatureAI::OnTargetDetected);
	// GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ACreatureAI::UpdatePerception);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void ACreatureAI::SetStunned(bool value)
{
	// bIsStunned = value;
	GetBlackboard()->SetValueAsBool(Stunned, value);
}

void ACreatureAI::SetCurrentSealStatus(Sealed Status)
{
	CurrentStatus = Status;
	
	GetBlackboard()->SetValueAsEnum(SealStatus, static_cast<uint8>(CurrentStatus));
}

Sealed ACreatureAI::GetCurrentSealStatus()
{
	//UAIPerceptionComponent* AIPerceptionComponent = GetAIPerceptionComponent();
 //   if (AIPerceptionComponent)
 //   {
 //       // AI Sight 센스 다시 활성화
 //       AIPerceptionComponent->RequestStimuliListenerUpdate();
 //   }
	return CurrentStatus;
}