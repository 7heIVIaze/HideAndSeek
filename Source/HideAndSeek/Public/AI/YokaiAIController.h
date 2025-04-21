// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.
// 모든 요괴 AI의 부모 클래스가 될 것임. 여기로 통합시킬 것.
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "YokaiAIController.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AYokaiAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AYokaiAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// AI가 Destroy될 경우 비헤이비어트리를 정리하고 Destroy되도록 하는 함수
	UFUNCTION(BlueprintCallable)
	virtual void StopAI();

	// AI Perception 설정하는 함수. Constructor에서 호출할 것
	UFUNCTION(BlueprintCallable)
	virtual void SetPerception();

	// 플레이어가 AI Perception의 주 감각에 의해 감지될 경우 처리할 함수
	UFUNCTION(BlueprintCallable)
	virtual void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	// 기절 상태가 되었다고 설정할 함수
	UFUNCTION(BlueprintCallable)
	virtual void SetStunned(bool IsStunned);

	// 추격이 끝났을 때, AI Sight를 재활성화하기 위한 함수.
	UFUNCTION(BlueprintCallable)
	virtual void EndChase();

	// 초기 블랙보드 컴포넌트와 비헤이비어트리 컴포넌트를 설정하는 함수. Constructor에서 호출할 것.
	UFUNCTION(BlueprintCallable)
	virtual void SetupBlackboardAndBehaviorTree();

public:
	static const FName PatrolTargetKey;
	static const FName NoiseTargetKey;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetLocation;
	static const FName CanSeePlayer;
	static const FName NoiseDetected;
	static const FName Stunned;
	static const FName LockerLighting;
	static const FName LockerTargetKey;

	// AI Perception Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation;

	// 불을 켜지 않았을 때, 감지 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoLightSightRadius;

protected:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	class UBlackboardComponent* BlackboardComp;
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTreeComponent* Behavior_Tree_Component;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	class UBlackboardData* BBAsset;

	UPROPERTY(VisibleAnywhere)
	bool bIsStunned;
};
