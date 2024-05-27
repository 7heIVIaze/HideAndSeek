// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#include "AI/BTService_Detect.h"
#include "AI/CreatureAI.h"
#include "AI/AIController_Runner.h"
#include "AI/AIController_Brute.h"
#include "AI/AIController_Shadow.h"
#include "AI/Reaper_cpp.h"
#include "AI/Runner_cpp.h"
#include "AI/Brute_cpp.h"
#include "AI/Shadow_cpp.h"
#include "HideAndSeek/HorrorGameCharacter.h"
#include "Cabinet_cpp.h"
#include "Wardrobe_cpp.h"
#include "Furniture/HideObject.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "HideAndSeek/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

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
	
	// 만약 그 컨트롤러가 Reaper의 AI일 경우
	if (ACreatureAI* ReaperAI = Cast<ACreatureAI>(AIController))
	{
		// 리퍼가 플레이어를 본 상태인지 확인하기 위해 그 값을 가져옴.
		AReaper_cpp* Reaper = Cast<AReaper_cpp>(ReaperAI->GetPawn());
		bool bDetected = ReaperAI->GetBlackboard()->GetValueAsBool(ACreatureAI::CanSeePlayer);
		if (nullptr == Reaper) return;

		UWorld* World = Reaper->GetWorld();

		// 매 틱마다 생성될 충돌체의 세부 사항들을 조정
		FVector Center = Reaper->GetActorLocation();
		float DetectRadius = 250.f;
		// 기본의 감지 범위는 300이지만 리퍼가 플레이어를 봤다면 1000으로 증가함.
		if (bDetected)
		{
			DetectRadius = 1000.f;
		}

		if (nullptr == World) return;
		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그가 있는 액터만을 검색하도록 검색 조건을 설정함.
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Reaper);
		
		// 콜리전을 생성하는데 구 모양으로 생성함.
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

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
						ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, PlayerCharacter);
						ReaperAI->GetBlackboard()->SetValueAsVector(ACreatureAI::TargetLocation, PlayerCharacter->GetActorLocation());

						// 디버깅용
						//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						//DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						//DrawDebugLine(World, Reaper->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						
						return;
					}
				}
				// Chack if a controller is player's controller
				/*if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, Cabinet);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
						}
						else
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, nullptr);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
						}
					}
				}
				if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, Wardrobe);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
						}
						else
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, nullptr);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
						}
					}
				}*/
				// 만약 그 객체가 숨는 액터라면(옷장/캐비닛)
				else if (AHideObject* HideObject = Cast<AHideObject>(OverlapResult.GetActor()))
				{
					// 플레이어가 그 곳에 숨은 상태면
					if (HideObject->bIsHiding)
					{
						// 불이 켜져있다면 해당 객체를 블랙보드의 Locker Target에 추가함
						if (HideObject->bIsFlashLightOn || HideObject->bIsCigarLightOn)
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, HideObject);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, true);
						}
						// 그게 아니라면 제거함.
						else
						{
							ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::LockerTargetKey, nullptr);
							ReaperAI->GetBlackboard()->SetValueAsBool(ACreatureAI::LockerLighting, false);
						}
					}
				}
				// 아무 것도 감지된 게 없다면 null로 초기화함.
				else
				{
					// set null
					ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			ReaperAI->GetBlackboard()->SetValueAsObject(ACreatureAI::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
		//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}
	// 만약 그 컨트롤러가 Runner의 AI일 경우
	if (AAIController_Runner* RunnerAI = Cast<AAIController_Runner>(AIController))
	{
		// 러너가 플레이어를 본 상태인지 확인하기 위해 그 값을 가져옴.
		ARunner_cpp* Runner = Cast<ARunner_cpp>(RunnerAI->GetPawn());
		bool bDetected = RunnerAI->GetBlackboard()->GetValueAsBool(AAIController_Runner::CanSeePlayer);
		if (nullptr == Runner) return;

		UWorld* World = Runner->GetWorld();
		if (nullptr == World) return;

		// 매 틱마다 생성될 충돌체의 세부 사항들을 조정
		FVector Center = Runner->GetActorLocation();
		// 기본 범위는 250이지만, 감지된 상태일 경우 범위를 1300으로 증가
		float DetectRadius = 250.f;
		if (bDetected)
		{
			DetectRadius = 1400.f;
		}

		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그가 있는 액터만을 검색하도록 조건을 추가함.
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Runner);
		
		// 콜리전을 생성하는데 구 모양으로 생성함.
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

		// 어떤 충돌이 감지되었을 경우
		if (bResult)
		{
			// 충돌된 객체 하나씩 검사함.
			for (auto OverlapResult : OverlapResults)
			{
				// 그 객체가 플레이어일 경우
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					// 숨은 상태가 아니라면 블랙보드의 target에 플레이어를 추가함.
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						UE_LOG(LogTemp, Warning, TEXT("Player Detected"));
						RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, PlayerCharacter);
						RunnerAI->GetBlackboard()->SetValueAsVector(AAIController_Runner::TargetLocation, PlayerCharacter->GetActorLocation());
						
						// 디버깅용
						// Show radius
						//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						//	DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						//	DrawDebugLine(World, Runner->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller
				/*if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							UE_LOG(LogTemp, Warning, TEXT("Cabinet Detected"));
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, Cabinet);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
						}
						else
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, nullptr);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
						}
					}
				}
				if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							UE_LOG(LogTemp, Warning, TEXT("Wardrobe Detected"));
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, Wardrobe);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
						}
						else
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, nullptr);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
						}
					}
				}*/
				// 만약 그 객체가 숨을 수 있는 액터라면(옷장/캐비닛)
				else if (AHideObject* HideObject = Cast<AHideObject>(OverlapResult.GetActor()))
				{
					// 플레이어가 그 곳에 숨은 상태면
					if (HideObject->bIsHiding)
					{
						// 불이 켜져있다면 해당 객체를 블랙보드의 Locker Target에 추가함
						if (HideObject->bIsFlashLightOn || HideObject->bIsCigarLightOn)
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, HideObject);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, true);
						}
						// 그게 아니라면 제거함.
						else
						{
							RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::LockerTargetKey, nullptr);
							RunnerAI->GetBlackboard()->SetValueAsBool(AAIController_Runner::LockerLighting, false);
						}
					}
				}
				// 아무것도 없다면 제거함
				else
				{
					// set null
					RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			RunnerAI->GetBlackboard()->SetValueAsObject(AAIController_Runner::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
		//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}
	// 만약 그 컨트롤러가 Brute의 AI라면
	if (AAIController_Brute* BruteAI = Cast<AAIController_Brute>(AIController))
	{
		// 브루트가 플레이어를 본 상태인지 확인하기 위해 그 값을 가져옴
		ABrute_cpp* Brute = Cast<ABrute_cpp>(BruteAI->GetPawn());
		bool bDetected = BruteAI->GetBlackboard()->GetValueAsBool(AAIController_Brute::CanSeePlayer);
		if (nullptr == Brute) return;

		UWorld* World = Brute->GetWorld();
		if (nullptr == World) return;

		// 매 틱마다 생성될 충돌체의 세부 사항들을 조정함.
		FVector Center = Brute->GetActorLocation();
		// 기본 범위는 250이지만 감지 시 1000으로 설정함
		float DetectRadius = 250.f;
		if (bDetected)
		{
			DetectRadius = 1000.f;
		}

		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그를 소유한 액터만 검색하도록 조건을 부여함
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Brute);
		
		// 충돌체를 구 모양으로 생성함.
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class

		// 어떤 충돌이 감지되었을 경우
		if (bResult)
		{
			// 충돌된 객체 하나씩 검사함
			for (auto OverlapResult : OverlapResults)
			{
				// 그 객체가 플레이어일 경우
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					// 숨은 상태가 아니라면 블랙보드의 Target에 추가함
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, PlayerCharacter);
						BruteAI->GetBlackboard()->SetValueAsVector(AAIController_Brute::TargetLocation, PlayerCharacter->GetActorLocation());

						// 디버깅용
						//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						// DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						// DrawDebugLine(World, Brute->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller
				/*if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, Cabinet);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, true);
						}
						else
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, nullptr);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
						}
					}
				}
				if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, Wardrobe);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, true);
						}
						else
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, nullptr);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
						}
					}
				}*/
				// 만약 그 객체가 숨을 수 있는 액터라면(옷장/캐비닛)
				else if (AHideObject* HideObject = Cast<AHideObject>(OverlapResult.GetActor()))
				{
					// 플레이어가 그 곳에 숨은 상태면
					if (HideObject->bIsHiding)
					{
						// 불이 켜져있다면 해당 객체를 블랙보드의 Locker Target에 추가함
						if (HideObject->bIsFlashLightOn || HideObject->bIsCigarLightOn)
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, HideObject);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, true);
						}
						// 그게 아니라면 제거함.
						else
						{
							BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::LockerTargetKey, nullptr);
							BruteAI->GetBlackboard()->SetValueAsBool(AAIController_Brute::LockerLighting, false);
						}
					}
				}
				else
				{
					// set null
					BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			BruteAI->GetBlackboard()->SetValueAsObject(AAIController_Brute::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
	//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}
	// 만약 그 컨트롤러가 Shadow의 AI라면
	if (AAIController_Shadow* ShadowAI = Cast<AAIController_Shadow>(AIController))
	{
		// 플레이어를 감지한 상태인지 확인하기 위해 그 값을 가져옴.
		AShadow_cpp* Shadow = Cast<AShadow_cpp>(ShadowAI->GetPawn());
		bool bDetected = ShadowAI->GetBlackboard()->GetValueAsBool(AAIController_Shadow::CanSeePlayer);
		if (nullptr == Shadow) return;

		UWorld* World = Shadow->GetWorld();
		if (nullptr == World) return;

		// 매 틱마다 생성될 충돌체의 세부 사항을 조정함.
		FVector Center = Shadow->GetActorLocation();
		// 기본 감지 범위를 250으로, 플레이러를 감지한 상태면 1000으로 늘림.
		float DetectRadius = 250.f;
		if (bDetected)
		{
			DetectRadius = 1000.f;
		}

		TArray<FOverlapResult> OverlapResults;
		// AICanDetect라는 태그를 가진 액터만을 검색하기 위해 조건을 추가함.
		FCollisionQueryParams CollisionQueryParam(TEXT("AICanDetect"), false, Shadow);
		
		// 충돌체를 구 모양으로 생성함.
		bool bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel3,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
		// ECC_GameTraceChannel3 is collision trace channel which is just detect overlapped only AHorrorGameCharacter class
		
		// 어떤 충돌이 감지되었을 경우
		if (bResult)
		{
			// 충돌된 객체 하나씩 검사함.
			for (auto OverlapResult : OverlapResults)
			{
				// 그 객체가 플레이어라면
				if (AHorrorGameCharacter* PlayerCharacter = Cast<AHorrorGameCharacter>(OverlapResult.GetActor()))
				{
					// 숨은 상태가 아니라면 블랙보드의 Target에 추가함
					if (PlayerCharacter && PlayerCharacter->GetController()->IsPlayerController() && !(PlayerCharacter->GetIsHiding()))
					{
						ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::TargetKey, PlayerCharacter);
						ShadowAI->GetBlackboard()->SetValueAsVector(AAIController_Shadow::TargetLocation, PlayerCharacter->GetActorLocation());
						
						// 디버깅용
						//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
						//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Red, false, 0.2f);
						//DrawDebugBox(World, Center, DetectSize, FColor::Green, false, 0.2f);

						//DrawDebugPoint(World, PlayerCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);
						//DrawDebugLine(World, Shadow->GetActorLocation(), PlayerCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
						return;
					}
				}
				// Chack if a controller is player's controller
				/*if (ACabinet_cpp* Cabinet = Cast<ACabinet_cpp>(OverlapResult.GetActor()))
				{
					if (Cabinet->bIsHiding)
					{
						if (Cabinet->bIsFlashLightOn || Cabinet->bIsCigarLightOn)
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, Cabinet);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, true);
						}
						else
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, nullptr);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, false);
						}
					}
				}
				if (AWardrobe_cpp* Wardrobe = Cast<AWardrobe_cpp>(OverlapResult.GetActor()))
				{
					if (Wardrobe->bIsHiding)
					{
						if (Wardrobe->bIsFlashLightOn || Wardrobe->bIsCigarLightOn)
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, Wardrobe);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, true);
						}
						else
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, nullptr);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, false);
						}
					}
				}*/
				// 만약 그 객체가 숨을 수 있는 액터라면(옷장/캐비닛)
				else if (AHideObject* HideObject = Cast<AHideObject>(OverlapResult.GetActor()))
				{
					// 플레이어가 그 곳에 숨은 상태면
					if (HideObject->bIsHiding)
					{
						// 불이 켜져있다면 해당 객체를 블랙보드의 Locker Target에 추가함
						if (HideObject->bIsFlashLightOn || HideObject->bIsCigarLightOn)
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, HideObject);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, true);
						}
						// 그게 아니라면 제거함.
						else
						{
							ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::LockerTargetKey, nullptr);
							ShadowAI->GetBlackboard()->SetValueAsBool(AAIController_Shadow::LockerLighting, false);
						}
					}
				}
				else
				{
					// set null
					ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::TargetKey, nullptr);
				}
			}
		}
		else
		{
			// set null
			ShadowAI->GetBlackboard()->SetValueAsObject(AAIController_Shadow::TargetKey, nullptr);
		}

		//DrawDebugCapsule(World, Center, 400.f, 20.f, FRotationMatrix::MakeFromZ(ControllingPawn->GetActorForwardVector()).ToQuat(), FColor::Green, false, 0.2f);
	//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
		//DrawDebugBox(World, Center, DetectSize, FColor::Red, false, 0.2f);
	}
}