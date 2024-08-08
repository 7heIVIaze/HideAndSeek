// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTTask_SetTrace.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/HorrorGameCharacter.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Shadow.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Shadow_cpp.h"

// AI의 추격 상태를 설정하는 로직
UBTTask_SetTrace::UBTTask_SetTrace()
{
	NodeName = TEXT("SetTrace");
}

EBTNodeResult::Type UBTTask_SetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Behavior Tree를 실행시키는 Component의 AI Controller를 가져옴
	AAIController* AIController = OwnerComp.GetAIOwner();

	// 그 컨트롤러가 Reaper의 컨트롤러라면
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());

		if (nullptr == Reaper)
		{
			return EBTNodeResult::Failed;
		}

		// 블랙보드에 저장된 Target을 가져옴
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(ReaperAI->GetBlackboard()->GetValueAsObject(ACreatureAI::TargetKey));

		// Target이 null이면 CanSeePlayer를 제거하고 추격을 멈춤
		if (nullptr == pTarget)
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, false);
			Reaper->EndChase();
		}
		// 반대라면 CanSeePlayer를 on하고 추격을 시작함.
		else
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::CanSeePlayer, true);
			Reaper->StartChase();
		}

		// 블랙보드에 저장된 Noise Detect를 가져옴
		bool bNoiseDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::NoiseDetected);

		// Noise Detect가 true라면 false로 지정함.
		if (bNoiseDetected)
		{
			ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::NoiseDetected, false);
		}

		return EBTNodeResult::Succeeded;
	}
	// 그 컨트롤러가 Runner의 컨트롤러면
	else if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());

		if (nullptr == Runner)
		{
			return EBTNodeResult::Failed;
		}

		// 블랙보드에 저장된 Target을 가져옴.
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(RunnerAI->GetBlackboard()->GetValueAsObject(AAIController_Runner::TargetKey));

		// Target이 null이라면 CanSeePlayer를 제거하고 추격을 멈춤
		if (nullptr == pTarget)
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, false);
			// RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
			Runner->EndChase();
		}
		// 반대라면 CanSeePlayer를 on하고 추격을 시작함.
		else
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::CanSeePlayer, true);
			Runner->StartChase();
		}
		
		// 블랙보드에 저장된 Noise Detect를 가져옴.
		bool bNoiseDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::NoiseDetected);

		// Noise Detect가 true라면 false로 변경함
		if (bNoiseDetected)
		{
			RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::NoiseDetected, false);
		}

		return EBTNodeResult::Succeeded;
	}
	// 그 컨트롤러가 Brute의 컨트롤러라면
	else if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());

		if (nullptr == Brute)
		{
			return EBTNodeResult::Failed;
		}

		// 블랙보드에 저장된 Target을 가져옴
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(BruteAI->GetBlackboard()->GetValueAsObject(AAIController_Brute::TargetKey));

		// Target이 null이라면 CanSeePlayer를 초기화하고 추격을 멈춤
		if (nullptr == pTarget)
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, false);
			// BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
			Brute->EndChase();
		}
		// 반대라면 CanSeePlayer를 on하고 추격을 시작함
		else
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::CanSeePlayer, true);
			Brute->StartChase();
		}

		// 블랙보드에 저장된 Noise Detect를 가져옴
		bool bNoiseDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::NoiseDetected);

		// true라면 Noise Detect를 false로 변경하고, 소리 감지 범위도 줄이도록 함.
		if (bNoiseDetected)
		{
			BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::NoiseDetected, false);
			Brute->ChangeNoiseRange(false);
		}

		return EBTNodeResult::Succeeded;
	}
	// 그 컨트롤러가 Shadow의 컨트롤러면
	else if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
	{
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());

		if (nullptr == Shadow)
		{
			return EBTNodeResult::Failed;
		}

		// 블랙보드에 저장된 Target을 가져옴
		AHorrorGameCharacter* pTarget = Cast<AHorrorGameCharacter>(ShadowAI->GetBlackboard()->GetValueAsObject(AAIController_Shadow::TargetKey));

		// Target이 null이라면 CanSeePlayer를 초기화하고 추격을 멈춤
		if (nullptr == pTarget)
		{
			ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::CanSeePlayer, false);
			// ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, false);
			Shadow->EndChase();
		}
		// 반대라면 CanSeePlayer를 on하고 추격을 시작함
		else
		{
			ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::CanSeePlayer, true);
			Shadow->StartChase();
		}

		// 블랙보드에 저장된 Noise Detect를 가져옴
		bool bNoiseDetected = ShadowAI->GetBlackboard()->GetValueAsBool(AAIController_Shadow::NoiseDetected);

		// true라면 Noise Detect를 false로 변경하고, 소리 감지 범위도 줄이도록 함.
		if (bNoiseDetected)
		{
			ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::NoiseDetected, false);
		}

		return EBTNodeResult::Succeeded;
		}

	return EBTNodeResult::Failed;
}