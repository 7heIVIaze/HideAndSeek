// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "Deprecated/BTService_SightDetect.h"
#include "AI/YokaiAIController.h"
#include "Player/HorrorGameCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_SightDetect::UBTService_SightDetect()
{
	NodeName = TEXT("SightDetect");
	FlashLightDist = 1600.f;
	CigarLightDist = 800.f;
	NoLightDist = 400.f;
	Interval = 0.5f;
}

void UBTService_SightDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(OwnerComp.GetAIOwner());
	APawn* Creature = nullptr;

}