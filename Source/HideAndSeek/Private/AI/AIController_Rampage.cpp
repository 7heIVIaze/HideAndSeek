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

// 블랙보드 데이터를 블루프린트로부터 읽어올 수 있도록 바인딩할 키를 지정해줌.
const FName AAIController_Rampage::IsCinematic(TEXT("IsCinematic"));
const FName AAIController_Rampage::CalledByBerith(TEXT("CalledByBerith"));

// Constructor
AAIController_Rampage::AAIController_Rampage()
{
	// 컨트롤러의 Tick은 비활성화시킴. 캐릭터만 활성화해도 됨.
	PrimaryActorTick.bCanEverTick = false;

	// AI Perception의 세부 데이터 초기화.
	AISightRadius = 1600.0f;
	AISightAge = 0.5f;
	AILoseSightRadius = 1700.0f;
	AIFieldOfView = 90.0f;
	AILastSeenLocation = 1200.0f;
	NoLightSightRadius = 1200.0f;
}

void AAIController_Rampage::SetupBlackboardAndBehaviorTree()
{
	Super::SetupBlackboardAndBehaviorTree();

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Assets/AI/Rampage/BB_Rampage"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BB Access Completed!"))
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Assets/AI/Rampage/BT_Rampage"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
		UE_LOG(LogTemp, Log, TEXT("BT Access Completed!"))
	}
}