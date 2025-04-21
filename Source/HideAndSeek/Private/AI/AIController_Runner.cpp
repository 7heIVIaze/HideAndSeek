// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

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
#include "Player/HorrorGameCharacter.h"

// Constructor
AAIController_Runner::AAIController_Runner()
{
	// AI Controller는 Tick 비활성화할 것임. Actor만 해도 충분함.
	PrimaryActorTick.bCanEverTick = false;
	
	// AI Perception의 세부 데이터 초기화.
	AISightRadius = 2400.0f;
	AISightAge = 0.5f;
	AILoseSightRadius = 2500.0f;
	AIFieldOfView = 80.0f;
	AILastSeenLocation = 2500.0f;
	NoLightSightRadius = 2000.0f;
}

// AI Perception 설정하는 함수. Constructor에서 호출할 것
void AAIController_Runner::SetPerception()
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
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Runner::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void AAIController_Runner::SetupBlackboardAndBehaviorTree()
{
	Super::SetupBlackboardAndBehaviorTree();
	
	// 블랙보드 컴포넌트를 블루프린트 클래스로부터 가져와 생성함.
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("/Game/Assets/AI/Runner/BB_Runner"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("/Game/Assets/AI/Runner/BT_Runner"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}