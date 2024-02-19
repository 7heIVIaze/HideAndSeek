// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/AIController_Rampage.h"
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
#include "AI/Rampage_cpp.h"

const FName AAIController_Rampage::PatrolTargetKey(TEXT("PatrolTarget"));
const FName AAIController_Rampage::NoiseTargetKey(TEXT("NoiseTargetKey"));
const FName AAIController_Rampage::HomePosKey(TEXT("HomePos"));
const FName AAIController_Rampage::PatrolPosKey(TEXT("PatrolPos"));
const FName AAIController_Rampage::TargetKey(TEXT("Target"));
const FName AAIController_Rampage::TargetLocation(TEXT("TargetLocation"));
const FName AAIController_Rampage::CanSeePlayer(TEXT("CanSeePlayer"));
const FName AAIController_Rampage::NoiseDetected(TEXT("NoiseDetected"));
const FName AAIController_Rampage::Stunned(TEXT("Stunned"));
const FName AAIController_Rampage::LockerLighting(TEXT("LockerLighting"));
const FName AAIController_Rampage::IsCinematic(TEXT("IsCinematic"));

AAIController_Rampage::AAIController_Rampage()
{
	PrimaryActorTick.bCanEverTick = false;
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Assets/AI/Rampage/BB_Rampage"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BB Access Completed!"))
	}

	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Assets/AI/Rampage/BT_Rampage"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BT Access Completed!"))
	}
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));

	// Initialize Perception
	SetPerception();
}

void AAIController_Rampage::BeginPlay()
{
	Super::BeginPlay();
	

	//CurrentStatus = Sealed::Sealed;
}


void AAIController_Rampage::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		RunBehaviorTree(BTAsset);
	}
}

void AAIController_Rampage::StopAI()
{
	// Super::OnUnPossess();

	UBehaviorTreeComponent* BehaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComp == nullptr) return;

	BehaviorTreeComp->StopTree(EBTStopMode::Safe);
}

UBlackboardComponent* AAIController_Rampage::GetBlackboard() const
{
	return BlackboardComp;
}
//void AAIController_Rampage::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

void AAIController_Rampage::UpdatePerception(const TArray<AActor*>& Actors)
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

void AAIController_Rampage::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
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
			if (player->GetIsHiding()) // 플레이어가 숨어있는 상태면, TargetKey 제거
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
					if (Dist <= 1200.f) // 1200 이하 거리 이내에 있으면 어느 상황에서든 감지
					{
						GetBlackboard()->SetValueAsObject(TargetKey, player);
						GetBlackboard()->SetValueAsVector(TargetLocation, player->GetActorLocation());
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
}

void AAIController_Rampage::SetPerception()
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Rampage::OnTargetDetected);
	// GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AAIController_Rampage::UpdatePerception);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIController_Rampage::SetStunned(bool value)
{
	// bIsStunned = value;
	GetBlackboard()->SetValueAsBool(Stunned, value);
}
