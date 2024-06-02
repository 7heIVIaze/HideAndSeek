// CopyrightNotice 2023 Sunggon Kim kimdave205@gmail.com. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIController_Runner.generated.h"

/**
 * 
 */
UCLASS()
class HIDEANDSEEK_API AAIController_Runner : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_Runner();

	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;
	UFUNCTION(BlueprintCallable)
		void StopAI();
	class UBlackboardComponent* GetBlackboard() const;

	UFUNCTION()
		void SetPerception();
	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	UFUNCTION()
		void SetStunned(bool value);

	UFUNCTION()
		void EndChase();
public:
	static const FName PatrolTargetKey;
	static const FName NoiseTargetKey;
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName TargetLocation;
	static const FName CanSeePlayer;
	static const FName NoiseDetected;
	//static const FName SprintDetected;
	static const FName Stunned;
	static const FName LockerLighting;
	static const FName LockerTargetKey;

	// AI Perception Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightRadius = 2400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AISightAge = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILoseSightRadius = 2500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AIFieldOfView = 80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AILastSeenLocation = 2500.0f;

private:
	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

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
