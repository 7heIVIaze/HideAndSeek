// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#include "AI/BTService_Detect.h"
#include "AI/YokaiAIController.h"
#include "AI/CreatureClass.h"
#include "Player/HorrorGameCharacter.h"
#include "Furniture/HideObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

// 생성자 - 플레이어가 감지 범위 내에 있을 때 추격을 계속하게 하기 위한 함수
UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

// 틱마다 발생시키는 함수임
void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 이 BT 서비스를 가지고 있는 AI의 컨트롤러를 가져옴.
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (AYokaiAIController* YokaiAI = Cast<AYokaiAIController>(AIController))
	{
		// 요괴가 플레이어를 본 상태인지 확인하기 위해 그 값을 가져옴.
		ACreatureClass* Yokai = Cast<ACreatureClass>(YokaiAI->GetPawn());
		bool bDetected = YokaiAI->GetBlackboardComponent()->GetValueAsBool(AYokaiAIController::CanSeePlayer);
		
		// 요괴 캐릭터를 가져오는데 실패했으면 종료.
		if (nullptr == Yokai)
		{
			return;
		}

		UWorld* World = Yokai->GetWorld();

		// 매 틱마다 생성될 충돌체의 세부 사항들을 조정함.
		FVector Center = Yokai->GetActorLocation();

		// 감지 범위는 기본 범위로 설정하지만, 플레이어를 본 이후엔 범위를 증가시킴.
		float DetectRadius = BaseRadius;
		if (bDetected)
		{
			DetectRadius = ExtendedRadius;
		}

		// 예외사항 설정
		if (nullptr == World)
		{
			return;
		}

		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그가 있는 액터만을 검색하도록 검색 조건을 설정함.
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Yokai);

		// 콜리전을 생성하는데 구 모양으로 생성함.
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);

		// 어떤 충돌이 감지되었을 경우
		if (bResult)
		{
			// 충돌된 객체를 하나하나씩 확인함.
			for (auto OverlapResult : OverlapResults)
			{
				// 만약 그 객체가 플레이어일 경우
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					// 플레이어가 숨은 상태가 아니라면, 플레이어에 대한 정보를 블랙보드의 Target에 저장시킴
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, PlayerCharacter);
						YokaiAI->GetBlackboardComponent()->SetValueAsVector(AYokaiAIController::TargetLocation, PlayerCharacter->GetActorLocation());

						// 디버깅용
						if (bIsDebug)
						{
							DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
							
							DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
							DrawDebugLine(World, Yokai->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						}
						return;
					}
				}
				// 만약 그 객체가 숨는 액터라면(옷장/캐비닛)
				else if (AHideObject* HideObject = Cast<AHideObject>(OverlapResult.GetActor()))
				{
					// 플레이어가 그 곳에 숨은 상태면
					if (HideObject->bIsHiding)
					{
						// 불이 켜져있다면 해당 객체를 블랙보드의 Locker Target에 추가함
						if (HideObject->bIsFlashLightOn || HideObject->bIsCigarLightOn)
						{
							YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::LockerTargetKey, HideObject);
							YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::LockerLighting, true);
						}
						// 그게 아니라면 제거함.
						else
						{
							YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::LockerTargetKey, nullptr);
							YokaiAI->GetBlackboardComponent()->SetValueAsBool(AYokaiAIController::LockerLighting, false);
						}
					}
				}
				// 아무 것도 감지된 게 없다면 null로 초기화함.
				else
				{
					// set null
					YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			YokaiAI->GetBlackboardComponent()->SetValueAsObject(AYokaiAIController::TargetKey, nullptr);
		}

		if (bIsDebug)
		{
			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		}
	}
}