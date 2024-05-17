// CopyrightNotice=0 2023 Sunggon Kim kimdave205@gmail.com

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController_Shadow.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Shadow : public AAIController
{
	GENERATED_BODY()
	
public:
	// Constructor
	AAIController_Shadow();

	void BeginPlay() override;

	void OnPossess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void StopAI();

	UFUNCTION(BlueprintCallable)
	class UBlackboardComponent* GetBlackboard() const;

	UFUNCTION()
	void SetPerception();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void SetStunned(bool value);

public:
	//////////////////////
	// BLACK BOARD DATA //
	//////////////////////
	static const FName PatrolTargetKey; // 패트롤 위치를 가진 액터를 저장할 Object 데이터
	static const FName NoiseTargetKey; // 소리를 내는 액터를 저장할 Object 데이터
	static const FName HomePosKey; // AI 캐릭터 자신 Object
	static const FName PatrolPosKey; // PatrolTargetKey의 위치를 저장할 FVector 데이터
	static const FName TargetKey; // 추격할 플레이어를 저장할 Object 데이터
	static const FName TargetLocation; // 추격할 플레이어의 위치나 소리를 내는 액터의 위치를 저장할 FVector 데이터
	static const FName CanSeePlayer; // 플레이어를 시야로 감지했는지 판단할 boolean 데이터
	static const FName NoiseDetected; // 플레이어나 액터의 소리를 감지했는지 판단할 boolean 데이터
	static const FName Stunned; // AI 캐릭터가 행동 불능 상태인지 판단할 boolean 데이터
	static const FName LockerLighting; // AI 캐릭터가 본 캐비닛/옷장 속 플레이어가 불을 켰는지 판단할 boolean 데이터
	static const FName LockerTargetKey; // AI 캐릭터가 본 플레이어가 불을 켠 캐비닛/옷장을 담은 Object 데이터

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightRadius = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AISightAge = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILoseSightRadius = 1700.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AIFieldOfView = 60.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AILastSeenLocation = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoLightSightRadius = 800.f;

private:
	class UBlackboardComponent* BlackboardComp;
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* Behavior_Tree_Component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	bool bIsStunned;
};
