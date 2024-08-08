// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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
#include "Player/HorrorGameCharacter.h"
#include "AI/Rampage_cpp.h"

// 블랙보드 데이터를 블루프린트로부터 읽을 수 있도록 바인딩해줌.
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
const FName AAIController_Rampage::CalledByBerith(TEXT("CalledByBerith"));

// Constructor
AAIController_Rampage::AAIController_Rampage()
{
	// 컨트롤러의 Tick은 비활성화시킴. 캐릭터만 활성화해도 됨.
	PrimaryActorTick.bCanEverTick = false;

	// 블랙보드 컴포넌트를 블루프린트 클래스로부터 가져와 생성함
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BB Comp"));
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Assets/AI/Rampage/BB_Rampage"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BB Access Completed!"))
	}

	// 비헤이비어 트리 컴포넌트를 블루프린트 클래스로부터 가져와 생성함.
	Behavior_Tree_Component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BT Comp"));
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Assets/AI/Rampage/BT_Rampage"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BT Access Completed!"))
	}

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

// AI Character가 Destroy될 때, 실행된 블랙보드와 비헤이비어 트리를 제거하고 Destroy하기 위한 함수.
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

// AI Perception의 주 감각을 통해 액터가 감지될 경우 수행할 콜백 함수.
void AAIController_Rampage::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	// 감지된 액터가 플레이어일 경우 수행함.
	if (auto const player = Cast<AHorrorGameCharacter>(Actor))
	{
		FTimerHandle Timer;
		if (Stimulus.WasSuccessfullySensed()) // 플레이어 감지했을 경우
		{
			// 플레이어와 램페이지의 위치를 가져옴
			FVector PlayerLocation = player->GetActorLocation();
			FVector RampageLocation = GetPawn()->GetActorLocation();
			RampageLocation.Z = 0.f;
			PlayerLocation.Z = 0.f;

			// 두 캐릭터 사이의 거리를 계산함.
			float Dist = FMath::Abs(FVector::Distance(RampageLocation, PlayerLocation));
			// 나중에 광원 종류 및 On 여부에 따라 감지 거리가 다르게 설정해야함

			GetWorld()->GetTimerManager().ClearTimer(Timer);
			
			if (player->GetIsHiding()) // 플레이어가 숨어있는 상태면, TargetKey 제거
			{
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}
			// 그것이 아니라면 감지된 플레이어는 조건 하나와 거리를 통해 발각 여부를 결정하게 됨.
			else
			{
				if (player->bIsCigarLightOn || player->bIsFlashLightOn) // 라이터든 플래시든 켰을 때는 시야 범위 내부에 있다면 무조건 감지
				{
					// GetBlackboard()->SetValueAsBool(CanSeePlayer, true);
					GetBlackboard()->SetValueAsObject(TargetKey, player);
					GetBlackboard()->SetValueAsVector(TargetLocation, player->GetActorLocation());
				}
				// 어떠한 불을 켜지도 않은 경우에는
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
		else // 플레이어 감지가 끝난 경우 5초 이후에 타겟 값에서 삭제
		{
			GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]() {
				GetBlackboard()->SetValueAsObject(TargetKey, nullptr);
			}), 1.f, false, 5.f);
		}
	}
}

// AI Perception을 설정하는 함수. Constructor에서 호출할 것.
void AAIController_Rampage::SetPerception()
{
	// AI Perception의 감각 기능 중 시각(Sight Config) 컴포넌트를 생성함.
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
